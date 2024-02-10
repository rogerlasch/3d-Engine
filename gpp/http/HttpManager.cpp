


#include<curl/curl.h>
#include<functional>
#include"../debug_system.h"
#include"../StateManager.h"
#include"CurlHandler.h"
#include"HttpRequest.h"
#include"httpPool.h"
#include"HttpManager.h"


using namespace std;

namespace gpp{
HttpManager::HttpManager(){
hpool=new HttpPool();
hstate.setState(0);
}

HttpManager::~HttpManager(){
stop();
lock_guard<mutex> lck(mtx);
for(auto it=handles.begin(); it!=handles.end(); ++it){
hpool->removeRequest(it->second);
delete it->second;
}
delete hpool;
hpool=NULL;
}

void HttpManager::start(){
if(hstate.getState()>0){
return;
}
try{
hstate.setState(1);
function<void()> hfunc=std::bind(&HttpManager::onLoop, this);
handle=thread(hfunc);
hstate.waitForStage([&](uint32 s)->bool{return s==0;});
} catch(const exception& e){
_GCRITICAL("{}", _GEXCEPT(e.what()).what());
}
}

void HttpManager::stop(){
if(hstate.getState()==0){
return;
}
hstate.setState(0);
hvar.notify_all();
if(handle.joinable()){
handle.join();
}
}

void HttpManager::setNotificationCallback(NOTIFICATION_HTTP_CALLBACK hcall){
this->hnotify=hcall;
}

NOTIFICATION_HTTP_CALLBACK HttpManager::getNotificationCallback()const{
return this->hnotify;
}

void HttpManager::setName(uint32 id, const string& name){
lock_guard<mutex> lck(mtx);
auto it=handles.find(id);
if(it!=handles.end()){
it->second->name=name;
}
}

string HttpManager::getName(uint32 id)const{
lock_guard<mutex> lck(mtx);
auto it=handles.find(id);
if(it!=handles.end()){
return it->second->name;
}
return "";
}

uint32 HttpManager::count()const{
lock_guard<mutex> lck(mtx);
return handles.size();
}

uint32 HttpManager::createRequest(const string& method, const string& url, const string& postFields, const vector<string>& headers){
if(hstate.getState()==0){
start();
}
HttpRequest* http=new HttpRequest();
http->method=method;
http->url=url;
http->postFields=postFields;
http->headers=headers;
uint32 res=http->start();
if(res!=HTTPREQUEST_ACTIVE){
delete http;
return 0;
}
else{
lock_guard<mutex> lck(mtx);
handles.insert(make_pair(http->getId(), http));
hpool->pushRequest(http);
hvar.notify_all();
}
return http->getId();
}

uint32 HttpManager::getResponse(uint32 id, string& response){
lock_guard<mutex> lck(mtx);
auto it=handles.find(id);
if(it==handles.end()){
return HTTPREQUEST_INVALID_HANDLE;
}

uint32 res=it->second->hstate.load();
switch(res){
case HTTPREQUEST_ACTIVE:{
return HTTPREQUEST_ACTIVE;
}
case HTTPREQUEST_FINISHED:
case HTTPREQUEST_ERROR:
{
response=it->second->response;
delete it->second;
handles.erase(it);
return res;
}
}
return HTTPREQUEST_UNKNOWN;
}

uint32 HttpManager::getState(uint32 id)const{
lock_guard<mutex> lck(mtx);
auto it=handles.find(id);
return ((it==handles.end()) ? HTTPREQUEST_INVALID_HANDLE : it->second->getState());
}

uint32 HttpManager::wait(uint32 id, string& response){
while(hstate.getState()!=0){
this_thread::yield();
lock_guard<mutex> lck(mtx);
auto it=handles.find(id);
if(it==handles.end()){
return HTTPREQUEST_INVALID_HANDLE;
}

uint32 res=it->second->hstate.load();
switch(res){
case HTTPREQUEST_FINISHED:
case HTTPREQUEST_ERROR:{
response=it->second->response;
delete it->second;
handles.erase(it);
return res;
}
}
}
return HTTPREQUEST_INVALID_HANDLE;
}

void HttpManager::onLoop(){
mutex mtx_loop;
unique_lock<mutex> lck_loop(mtx_loop);
hstate.setStage(1);
while(hstate.getState()!=0){
this_thread::yield();
if(count()==0){
hvar.wait(lck_loop, [&]() {
if(count()==0){
if(hstate.getState()>0){
return false;
}
}
return true;
});
}
vector<uint32> ids;
hpool->update(ids);
if(ids.size()>0){
for(auto& it : ids){
this_thread::yield();
lock_guard<mutex> lck(mtx);
auto hf=handles.find(it);
if(hf==handles.end()){
continue;
}
hf->second->hstate.store(HTTPREQUEST_FINISHED);
}
if(hnotify){
for(auto& it : ids){
hnotify(it);
}
}
}
}
}
}

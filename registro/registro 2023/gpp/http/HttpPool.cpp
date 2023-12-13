

#include<curl/curl.h>
#include<atomic>
#include"CurlHandler.h"
#include"../gpp/debug_system.h"
#include"HttpRequest.h"
#include"HttpPool.h"


using namespace std;

namespace gpp{
atomic<uint32> HttpPool::refcount=0;
HttpPool::HttpPool(){
mhandle=NULL;
if(refcount.load()==1){
int32 res=curl_global_init(CURL_GLOBAL_ALL);
if(res!=0)
{
string msg=safe_format("Erro ao inicializar a libcurl. Erro: {}", res);
throw _GEXCEPT(msg);
}
}
mhandle=curl_multi_init();
actives.store(0);
}

HttpPool::~HttpPool(){
actives.store(0);
curl_multi_cleanup(mhandle);
mhandle=NULL;
if(refcount.load()>0){
refcount.fetch_sub(1);
if(refcount.load()==0){
curl_global_cleanup();
}
}
}

uint32 HttpPool::getActives()const{
return actives.load();
}

void HttpPool::pushRequest(HttpRequest* req){
lock_guard<mutex> lck(mtx);
curl_multi_add_handle(mhandle, req->handle->handle);
actives.fetch_add(1);
}

void HttpPool::removeRequest(HttpRequest* req){
lock_guard<mutex> lck(mtx);
curl_multi_remove_handle(mhandle, req->handle->handle);
actives.fetch_sub(1);
}

void HttpPool::update(vector<uint32>& ids){
ids.clear();
if(actives.load()==0)return;
lock_guard<mutex> lck(mtx);
int32 x=0;
int32 msg_left=0;
CURLMsg* msg=NULL;
CURLMcode res=curl_multi_perform(mhandle, &x);
while((msg=curl_multi_info_read(mhandle, &msg_left))){
    if(msg->msg == CURLMSG_DONE) {
    uint32* id=NULL;
    curl_easy_getinfo(msg->easy_handle, CURLINFO_PRIVATE, &id);

ids.push_back((*id));
    curl_multi_remove_handle(mhandle, msg->easy_handle);
    if(actives.load()>0){
    actives.fetch_sub(1);
    }
    }
}
}
}

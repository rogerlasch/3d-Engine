

#include<curl/curl.h>
#include<sstream>
#include"../gpp/debug_system.h"
#include"../string_utils.h"
#include"CurlHandler.h"
#include"HttpRequest.h"


using namespace std;

namespace gpp{
static void internal_curl_cleanup(CurlHandler* h){
_GASSERT(h!=NULL);

if(h->handle!=NULL){
curl_easy_cleanup(h->handle);
}

if(h->headers!=NULL){
curl_slist_free_all(h->headers);
}
h->handle=NULL;
h->headers=NULL;
}

//The HttpRequest Class

atomic<uint32> HttpRequest::refcount=1000;
HttpRequest::HttpRequest() {
refcount.fetch_add(1);
this->id=refcount.load();
handle=new CurlHandler();
this->cleanup();
}

HttpRequest::~HttpRequest(){
this->cleanup();
delete handle;
handle=NULL;
}

void HttpRequest::cleanup(){
this->code=200;
this->name="";
this->method="";
this->url="";
this->postFields="";
this->body="";
this->response="";
this->headers.clear();
internal_curl_cleanup(handle);
}

string HttpRequest::toString(){
stringstream ss;
ss<<fixed;
ss.precision(2);
ss<<"Id:"<<id<<endl;
ss<<"Url:"<<url<<endl;
ss<<"PostFields:"<<postFields<<endl;
ss<<"Body:"<<body<<endl;
ss<<"Headers:"<<endl;
for(auto& it : headers){
ss<<it<<endl;
}
return ss.str();
}

uint32 HttpRequest::getId()const{
return this->id;
}

uint32 HttpRequest::getState()const{
return hstate.load();
}

uint32 HttpRequest::start(){
switch(hstate.load()){
case HTTPREQUEST_DEFAULT:
case HTTPREQUEST_FINISHED:
case HTTPREQUEST_ERROR:
{
internal_curl_cleanup(handle);
hstate.store(HTTPREQUEST_ACTIVE);
string_to_upper_case(method);
handle->handle=curl_easy_init();
curl_easy_setopt(handle->handle, CURLOPT_PRIVATE, &id);
curl_easy_setopt(handle->handle, CURLOPT_URL, url.c_str());
  curl_easy_setopt(handle->handle, CURLOPT_WRITEFUNCTION, http_push_memory_callback);
  curl_easy_setopt(handle->handle, CURLOPT_WRITEDATA, &response);
  curl_easy_setopt(handle->handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
curl_easy_setopt(handle->handle, CURLOPT_FOLLOWLOCATION, 1L);
curl_easy_setopt(handle->handle, CURLOPT_TIMEOUT, 20L);
curl_easy_setopt(handle->handle, CURLOPT_CONNECTTIMEOUT, 5L);
curl_easy_setopt(handle->handle, CURLOPT_CUSTOMREQUEST, method.c_str());
curl_easy_setopt(handle->handle, CURLOPT_POSTFIELDS, postFields.c_str());
curl_easy_setopt(handle->handle, CURLOPT_POSTFIELDSIZE, postFields.size());
if(headers.size()>0){
for(auto& it : headers){
handle->headers=curl_slist_append(handle->headers, it.c_str());
}
curl_easy_setopt(handle->handle, CURLOPT_HTTPHEADER, handle->headers);
}
return hstate.load();
}
}
return HTTPREQUEST_ALREADY;
}

size_t HttpRequest::http_push_memory_callback(void* ptr, size_t size, size_t nmemb, void *userp)
{
(*(string*)userp).append((char*)ptr, nmemb);
  return nmemb;
}
}

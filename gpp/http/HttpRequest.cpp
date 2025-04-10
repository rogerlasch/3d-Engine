
#include <curl/curl.h>
#include "../debug_system.h"
#include"../string_utils.h"
#include <sstream>
#include "HttpService.h"
#include "CurlData.h"
#include "HttpRequest.h"

namespace gpp {

std::atomic<uint32> HttpRequest::refcount = 0;

HttpRequest::HttpRequest() {
    id = refcount.fetch_add(1);
    code = 0;
    method = "GET";
    url.clear();
    body.clear();
    response.clear();
    errormsg.clear();
errorBuffer.clear();
    headers.clear();
debug_buffer.str().clear();
    hstate.store(HTTPREQUEST_DEFAULT);
hcall=NULL;
    hdata = new CurlData();
    hservice = nullptr;
}

HttpRequest::~HttpRequest() {
    release();
    delete hdata;
    hdata = nullptr;
}

std::string HttpRequest::toString() const {
    std::stringstream ss;
    ss << "id=" << id << ", Method=" << method << ", Url=" << url << "\n";
    ss << "Code=" << code << ", Response=\"" << response << "\"\n";
ss<<"UserAgent=\""<<userAgent<<"\"\n";
    ss << "Body=\"" << body << "\"\n";
    ss << "Headers={\n";
    for (const auto& it : headers) {
        ss << it << "\n";
    }
    ss << "}\nErrormsg=" << errormsg << "\n";
    return ss.str();
}

void HttpRequest::cleanup() {
    setCode(200);
    setMethod("GET");
    setUrl("");
    setBody("");
    setResponse("");
    setErrorMessage("");
    setHeaders({});
    setState(HTTPREQUEST_DEFAULT);
hcall=NULL;
errorBuffer.clear();
debug_buffer.str().clear();
    release();
}

void HttpRequest::release() {
    if (hservice != nullptr) {
        hservice->removeRequest(id);
        hservice = nullptr;
    }
}

void HttpRequest::waitForCompletion() {
    while (getState() == HTTPREQUEST_ACTIVE) {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}

    void HttpRequest::setMethod(const std::string& m){
this->method = string_to_upper_case(m);
}

void HttpRequest::executeCallback(){
if(this->hcall){
hcall(this);
}
}
}

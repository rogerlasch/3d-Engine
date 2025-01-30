
#include <curl/curl.h>
#include "../debug_system.h"
#include <sstream>
#include "HttpService.h"
#include "CurlData.h"
#include "HttpRequest.h"

namespace gpp {

std::atomic<uint32> HttpRequest::refcount = 0;

HttpRequest::HttpRequest() {
    id = refcount.fetch_add(1);
    code = 0;
    method = "get";
    url = "";
    body = "";
    response = "";
    errormsg = "";
    headers.clear();
    hstate.store(HTTPREQUEST_DEFAULT);
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
    setMethod("get");
    setUrl("");
    setBody("");
    setResponse("");
    setErrorMessage("");
    setHeaders({});
    setState(HTTPREQUEST_DEFAULT);
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
} // namespace gpp

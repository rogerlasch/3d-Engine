
#include "../debug_system.h"
#include"HttpRequest.h"
#include"CurlHandler.h"
#include "HttpService.h"

using namespace std;

namespace gpp {

HttpService::HttpService()
    : hstate(HTTPSERVICE_DEFAULT),
      hcurl(make_unique<CurlHandler>()) {} // Uso de unique_ptr para evitar delete manual

HttpService::~HttpService() {
    stop();
}

shared_HttpRequest HttpService::createNewRequest(){
return make_shared<HttpRequest>();
}

shared_HttpRequest HttpService::createRequest(const string& method, const string& url,                                               const string& postFields,                                               const vector<string>& headers, const string& userAgent) {
    auto request = createNewRequest();
    request->setMethod(method);
    request->setUrl(url);
request->setUserAgent(userAgent);
    request->setBody(postFields);
    request->setHeaders(headers);

    if (!pushRequest(request)) {
        return nullptr;
    }

    return request;
}

bool HttpService::pushRequest(shared_HttpRequest& hrequest) {
    try {
        scoped_lock lck(mtx, hrequest->mtx);

        if (hcurl->pushRequest(hrequest)) {
            hrequest->setService(this);
            requests[hrequest->getId()] = hrequest;
            return true;
        }
    } catch (const exception& e) {
logger::info("Exception in {}.\nError: {}.\nStackTrace:\n{}", __FUNCTION__, e.what(), getStackTrace());
    }
    return false;
}

bool HttpService::removeRequest(uint32 id) {
    try {
        lock_guard<mutex> lck(mtx);
        auto it = requests.find(id);
        if (it != requests.end()) {
            hcurl->removeRequest(it->second);
it->second->setService(NULL);
            requests.erase(it);
            return true;
        }
    } catch (const exception& e) {
logger::info("Exception in {}.\nError: {}.\nStackTrace:\n{}", __FUNCTION__, e.what(), getStackTrace());
    }
    return false;
}

void HttpService::start() {
    try {
        lock_guard<mutex> lck(mtx);
        if (hstate.load() == HTTPSERVICE_ACTIVE || hstate.load() == HTTPSERVICE_STOPPING) {
            return;
        }

        handle = jthread([this](stop_token token) {
            try {
                hstate.store(HTTPSERVICE_ACTIVE);

                while (hstate.load() == HTTPSERVICE_ACTIVE) {
                    this_thread::sleep_for(chrono::milliseconds(100));
                    if (token.stop_requested()) {
                        break;
                    }
                    hcurl->update();
                }

                hstate.store(HTTPSERVICE_STOPPED);
            } catch (const exception& e) {
        logger::info("Exception in {}.\nError: {}.\nStackTrace:\n{}", __FUNCTION__, e.what(), getStackTrace());
                hstate.store(HTTPSERVICE_DEFAULT);
            }
        });

        while (hstate.load() != HTTPSERVICE_ACTIVE) {
            this_thread::sleep_for(chrono::milliseconds(20));
        }
    } catch (const exception& e) {
logger::info("Exception in {}.\nError: {}.\nStackTrace:\n{}", __FUNCTION__, e.what(), getStackTrace());
        throw;
    }
}

void HttpService::stop() {
    if (hstate.load() == HTTPSERVICE_ACTIVE) {
        hstate.store(HTTPSERVICE_STOPPING);
        if (handle.joinable()) {
            handle.request_stop();
            handle.join();
        }
        stopAll();
    }
}

void HttpService::stopAll() {
    hcurl->stopAll();
    while (!requests.empty()) {
        removeRequest(requests.begin()->first);
    }
}

string url_encode(const string& s) {
    CURL* curl = curl_easy_init();
    if (curl) {
        char* output = curl_easy_escape(curl, s.c_str(), s.length());
        string encoded(output);
        curl_free(output);
        curl_easy_cleanup(curl);
        return encoded;
    }
    return "";
}

// Função para decodificar uma string codificada de URL
string url_decode(const string& encoded) {
    CURL* curl = curl_easy_init();
    if (curl) {
        int out_length = 0;
        char* output = curl_easy_unescape(curl, encoded.c_str(), encoded.length(), &out_length);
        string decoded(output, out_length);
        curl_free(output);
        curl_easy_cleanup(curl);
        return decoded;
    }
    return "";
}
}

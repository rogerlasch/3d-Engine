
#ifndef HTTPSERVICE_H
#define HTTPSERVICE_H

#include "HttpRequest.h"
#include <atomic>
#include <unordered_map>
#include <thread>
#include <vector>
#include <mutex>
#include <memory>

namespace gpp {

enum HTTPSERVICE_STATES {
    HTTPSERVICE_DEFAULT = 0,
    HTTPSERVICE_ACTIVE,
    HTTPSERVICE_STOPPING,
    HTTPSERVICE_STOPPED
};

class CurlHandler; // Forward declaration

class HttpService {
private:
    std::atomic<uint32_t> hstate;
    std::unordered_map<uint32_t, shared_HttpRequest> requests;
    std::jthread handle;
    std::mutex mtx;
    std::unique_ptr<CurlHandler> hcurl; // Gerenciamento automático de memória

public:
    HttpService();
    HttpService(const HttpService& hs) = delete;
    HttpService& operator=(const HttpService& hs) = delete;
    ~HttpService();

shared_HttpRequest createNewRequest();
    shared_HttpRequest createRequest(const std::string& method, const std::string& url,                                      const std::string& postFields="",                                      const std::vector<std::string>& headers={}, const std::string& userAgent="");
    bool pushRequest(shared_HttpRequest& hrequest);
    bool removeRequest(uint32_t id);
    void start();
    void stop();

private:
    void stopAll();
};

std::string url_encode(const std::string& s);
std::string url_decode(const std::string& encoded);

} // namespace gpp
#endif // HTTPSERVICE_H

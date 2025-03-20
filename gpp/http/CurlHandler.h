
#ifndef CURLHANDLER_H
#define CURLHANDLER_H

#include <curl/curl.h>
#include <unordered_map>
#include <atomic>
#include <memory>
#include <mutex>
#include <string>
#include "CurlData.h"
#include "HttpRequest.h"

namespace gpp {

class CurlHandler {
private:
    CURLM* handle; // Multi-handle da libcurl
    std::unordered_map<CURL*, shared_HttpRequest> hrequests; // Mapeamento de handles para requisições
    mutable std::mutex mtx; // Protege acesso a hrequests
    static std::atomic<uint32_t> refcount; // Contador de referências para inicialização global

public:
    CurlHandler();
    CurlHandler(const CurlHandler& ch) = delete;
    CurlHandler& operator=(const CurlHandler& ch) = delete;
    ~CurlHandler();

    // Retorna o número de requisições ativas
    uint32_t size() const {
        std::lock_guard<std::mutex> lock(mtx);
        return hrequests.size();
    }

    void removeRequest(shared_HttpRequest& hrequest);
    bool pushRequest(shared_HttpRequest& hrequest);
    void update();
    void stopAll();

private:
    void cleanupRequest(CurlData* hdata);

    // Callback para processar dados recebidos
    static size_t httpPushMemoryCallback(void* ptr, size_t size, size_t nmemb, void* userp);
static int32 debug_callback(CURL *handle,                   curl_infotype type,                   char *data,                   size_t size,                   void *userptr);
};
} // namespace gpp
#endif // CURLHANDLER_H


#include "../debug_system.h"
#include"../string_utils.h"
#include "CurlHandler.h"


using namespace std;

namespace gpp {

atomic<uint32> CurlHandler::refcount = 0;
CurlHandler::CurlHandler() {
    refcount.fetch_add(1);
    if (refcount.load() == 1) {
        int32 res = curl_global_init(CURL_GLOBAL_ALL);
        if (res != 0) {
            throw runtime_error(safe_format("Erro ao inicializar a libcurl. Erro: {}", res));
        }
    }
    handle = curl_multi_init();
    if (!handle) {
        throw runtime_error("Erro ao inicializar CURL_MULTI_HANDLE");
    }
}

// Destrutor
CurlHandler::~CurlHandler() {
    stopAll();
    curl_multi_cleanup(handle);
    handle = nullptr;

    if (refcount.load() > 0) {
        refcount.fetch_sub(1);
        if (refcount.load() == 0) {
            curl_global_cleanup();
        }
    }
}

// Adiciona uma nova requisição ao CURLM
bool CurlHandler::pushRequest(shared_HttpRequest& hrequest) {
    auto hdata = hrequest->hdata;
    hdata->handle = curl_easy_init();
    if (!hdata->handle) {
hrequest->setState(HTTPREQUEST_ERROR);
        hrequest->setErrorMessage("Erro ao chamar curl_easy_init.");
        return false;
    }

    curl_easy_setopt(hdata->handle, CURLOPT_URL, hrequest->getUrl().c_str());
    curl_easy_setopt(hdata->handle, CURLOPT_WRITEFUNCTION, httpPushMemoryCallback);
    curl_easy_setopt(hdata->handle, CURLOPT_WRITEDATA, &hrequest->response);

if(hrequest->getUserAgent().size()==0){
    curl_easy_setopt(hdata->handle, CURLOPT_USERAGENT, "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/85.0.4183.102 Safari/537.36,gzip(gfe)");
}
else{
    curl_easy_setopt(hdata->handle, CURLOPT_USERAGENT, hrequest->getUserAgent().c_str());
}
    curl_easy_setopt(hdata->handle, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(hdata->handle, CURLOPT_TIMEOUT, 20L);
    curl_easy_setopt(hdata->handle, CURLOPT_CONNECTTIMEOUT, 5L);
    curl_easy_setopt(hdata->handle, CURLOPT_CUSTOMREQUEST, hrequest->getMethod().c_str());
curl_easy_setopt(hdata->handle, CURLOPT_SSL_OPTIONS, CURLSSLOPT_NO_REVOKE);

hrequest->errorBuffer.resize(CURL_ERROR_SIZE);
curl_easy_setopt(hdata->handle, CURLOPT_ERRORBUFFER, (char*)hrequest->errorBuffer.c_str());
curl_easy_setopt(hdata->handle, CURLOPT_DEBUGDATA, (void*)&hrequest->debug_buffer);
curl_easy_setopt(hdata->handle, CURLOPT_DEBUGFUNCTION, debug_callback);
    curl_easy_setopt(hdata->handle, CURLOPT_VERBOSE, 1L);

if(hrequest->getBody().size()>0){
    curl_easy_setopt(hdata->handle, CURLOPT_POST, 1L);
        curl_easy_setopt(hdata->handle, CURLOPT_COPYPOSTFIELDS, hrequest->getBody().c_str());
}

    if (!hrequest->getHeaders().empty()) {
        for (const auto& header : hrequest->getHeaders()) {
            hdata->headers = curl_slist_append(hdata->headers, header.c_str());
        }
        curl_easy_setopt(hdata->handle, CURLOPT_HTTPHEADER, hdata->headers);
    }

try     {
        lock_guard<mutex> lock(mtx);

        CURLMcode res = curl_multi_add_handle(handle, hdata->handle);
        if (res != CURLM_OK) {
hrequest->setState(HTTPREQUEST_ERROR);
            hrequest->setErrorMessage(hrequest->errorBuffer);
            cleanupRequest(hdata);
            return false;
        }

        hrequests[hdata->handle] = hrequest;
hrequest->setState(HTTPREQUEST_ACTIVE);
    } catch(const exception& e){
string msg=safe_format("Exception in {}.\nError: {}.\nStackTrace:\n{}", __FUNCTION__, e.what(), getStackTrace());
logger::info(msg);
hrequest->setState(HTTPREQUEST_ERROR);
hrequest->setErrorMessage(msg);
cleanupRequest(hrequest->hdata);
return false;
}
    return true;
}

void CurlHandler::removeRequest(shared_HttpRequest& hrequest) {
    lock_guard<mutex> lck(mtx);

    auto it = hrequests.find(hrequest->hdata->handle);
    if (it != hrequests.end()) {
        cleanupRequest(hrequest->hdata);
        if (hrequest->getState() != HTTPREQUEST_FINISHED) {
            hrequest->setState(HTTPREQUEST_CANCELED);
hrequest->setErrorMessage("A requisição foi cancelada.");
        }
        hrequests.erase(it);
    }
}

void CurlHandler::update() {

try {
lock_guard<mutex> lck(mtx);
    int still_running = 0;

    CURLMcode res = curl_multi_perform(handle, &still_running);
    if (res != CURLM_OK) {
        throw runtime_error(             safe_format("Erro ao executar curl_multi_perform. Código: {}", (int32)res));
    }

    int msgs_in_queue = 0;
    CURLMsg* msg = nullptr;
    while ((msg = curl_multi_info_read(handle, &msgs_in_queue))) {
        if (msg->msg == CURLMSG_DONE) {
            CURL* easy_handle = msg->easy_handle;
            auto it = hrequests.find(easy_handle);
            if (it != hrequests.end()) {
                auto hrequest = it->second;
                long response_code = 0;
                curl_easy_getinfo(easy_handle, CURLINFO_RESPONSE_CODE, &response_code);
                hrequest->setCode(static_cast<uint32>(response_code));

                if (msg->data.result != CURLE_OK) {
                    hrequest->setErrorMessage(curl_easy_strerror(msg->data.result));
                }

                cleanupRequest(hrequest->hdata);
                hrequests.erase(it);
                hrequest->setState(HTTPREQUEST_FINISHED);
hrequest->executeCallback();
            }
        }
    }
}catch(const exception& e){
logger::info("Exception in {}.\nError: {}.\nStackTrace:\n{}", __FUNCTION__, e.what(), getStackTrace());
}
}

// Limpa os dados associados a um CurlData
void CurlHandler::cleanupRequest(CurlData* hdata) {
    if (hdata) {
        if (hdata->handle) {
            curl_multi_remove_handle(handle, hdata->handle);
            curl_easy_cleanup(hdata->handle);
            hdata->handle = nullptr;
        }
        if (hdata->headers) {
            curl_slist_free_all(hdata->headers);
            hdata->headers = nullptr;
        }
    }
}

void CurlHandler::stopAll() {
try{
    lock_guard<mutex> lock(mtx);

    for (auto& pair : hrequests) {
        cleanupRequest(pair.second->hdata);
        if (pair.second->getState() != HTTPREQUEST_FINISHED) {
            pair.second->setState(HTTPREQUEST_CANCELED);
pair.second->setErrorMessage("A requisição foi cancelada pois stopAll foi chamado.");
        }
    }

    hrequests.clear();
}catch(const exception& e){
logger::info("Exception in {}.\nError: {}.\nStackTrace:\n{}", __FUNCTION__, e.what(), getStackTrace());
}
}

// Callback para manipular os dados recebidos
size_t CurlHandler::httpPushMemoryCallback(void* ptr, size_t size, size_t nmemb, void* userp) {
    auto* response = static_cast<string*>(userp);
    response->append(static_cast<char*>(ptr), size * nmemb);
    return  nmemb;
}

int32 CurlHandler::debug_callback(CURL *handle,                   curl_infotype type,                   char *data,                   size_t size,                   void *userptr){
    std::stringstream *ss = static_cast<std::stringstream *>(userptr);
    switch (type) {
        case CURLINFO_TEXT:
            *ss << "[INFO] " << std::string(data, size);
            break;

        case CURLINFO_HEADER_IN:
            *ss << "[HEADER IN] " << std::string(data, size);
            break;

        case CURLINFO_HEADER_OUT:
            *ss << "[HEADER OUT] " << std::string(data, size);
            break;

        case CURLINFO_DATA_IN:
            *ss << "[DATA IN] " << size << " bytes recebidos" << std::endl;
//            *ss << std::string(data, size) << std::endl;  // Dados recebidos
            break;

        case CURLINFO_DATA_OUT:
            *ss << "[DATA OUT] " << size << " bytes enviados" << std::endl;
//            *ss << std::string(data, size) << std::endl;  // Dados enviados
            break;

        case CURLINFO_SSL_DATA_IN:
            *ss << "[SSL DATA IN] " << size << " bytes recebidos (SSL)" << std::endl;
            break;

        case CURLINFO_SSL_DATA_OUT:
            *ss << "[SSL DATA OUT] " << size << " bytes enviados (SSL)" << std::endl;
            break;

        default:
            *ss << "[UNKNOWN] Tipo desconhecido: " << type << std::endl;
            break;
    }
return 0;
}
}

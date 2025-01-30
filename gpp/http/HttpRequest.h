
#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <atomic>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

namespace gpp {

enum HTTPREQUEST_STATUS {
    HTTPREQUEST_DEFAULT = 0,
    HTTPREQUEST_ACTIVE,
    HTTPREQUEST_ERROR,
    HTTPREQUEST_CANCELED,
    HTTPREQUEST_FINISHED
};

class CurlHandler;
class CurlData;
class HttpService;

class HttpRequest {
private:
    uint32 id;
    std::atomic<uint32> hstate;
    CurlData* hdata;
    HttpService* hservice;
    static std::atomic<uint32> refcount;

    uint32 code;
    std::string method;
    std::string url;
    std::string body;
    std::string response;
    std::string errormsg;
    std::vector<std::string> headers;
    std::mutex mtx;

public:
    HttpRequest();
    HttpRequest(const HttpRequest& http) = delete;
    HttpRequest& operator=(const HttpRequest& http) = delete;
    ~HttpRequest();

    // Getters e setters
    uint32 getId() const { return id; }
    uint32 getState() const { return hstate.load(); }
    void setState(uint32 state) { hstate.store(state); }

    uint32 getCode() const { return code; }
    void setCode(uint32 c) { code = c; }

    std::string getMethod() const { return method; }
    void setMethod(const std::string& m) { method = m; }

    std::string getUrl() const { return url; }
    void setUrl(const std::string& u) { url = u; }

    std::string getBody() const { return body; }
    void setBody(const std::string& b) { body = b; }

    std::string getResponse() const { return response; }
    void setResponse(const std::string& r) { response = r; }

    std::string getErrorMessage() const { return errormsg; }
    void setErrorMessage(const std::string& e) { errormsg = e; }

    std::vector<std::string> getHeaders() const { return headers; }
    void setHeaders(const std::vector<std::string>& h) { headers = h; }

    HttpService* getService() const { return hservice; }
    void setService(HttpService* service) { hservice = service; }

    std::string toString() const;
    void cleanup();
    void release();
    void waitForCompletion();

private:
    friend class HttpService;
    friend class CurlHandler;
};

typedef std::shared_ptr<HttpRequest> shared_HttpRequest;
} // namespace gpp
#endif // HTTPREQUEST_H

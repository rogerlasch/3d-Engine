
#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <atomic>
#include <memory>
#include <mutex>
#include <string>
#include<sstream>
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
typedef std::function<void(HttpRequest*)> HTTPREQUESTCALLBACK;
private:
    uint32 id;
    uint32 code;
    std::string method;
    std::string url;
    std::string body;
std::string userAgent;
    std::string response;
    std::string errormsg;
    std::vector<std::string> headers;

private:
std::string errorBuffer;
std::stringstream debug_buffer;
HTTPREQUESTCALLBACK hcall;
    CurlData* hdata;
    std::atomic<uint32> hstate;
    HttpService* hservice;
    static std::atomic<uint32> refcount;
    std::mutex mtx;

public:
    HttpRequest();
    HttpRequest(const HttpRequest& http) = delete;
    HttpRequest& operator=(const HttpRequest& http) = delete;
    ~HttpRequest();

    // Getters e setters
inline     uint32 getId() const { return this->id; }
inline     uint32 getState() const { return this->hstate.load(); }
inline     void setState(uint32 state) { this->hstate.store(state); }

inline     uint32 getCode() const { return this->code; }
inline     void setCode(uint32 c) { this->code = c; }

inline const std::string& getMethod() const { return this->method; }
    void setMethod(const std::string& m);

inline const std::string& getUrl() const { return this->url; }
inline     void setUrl(const std::string& url) { this->url = url; }

inline const std::string& getBody() const { return this->body; }
inline     void setBody(const std::string& body) { this->body = body; }

inline const std::string& getResponse() const { return this->response; }
inline     void setResponse(const std::string& resp) { this->response = resp; }

inline const std::string& getErrorMessage() const { return this->errormsg; }
inline     void setErrorMessage(const std::string& e) { this->errormsg = e; }

inline const std::string& getUserAgent()const{return this->userAgent;}
inline void setUserAgent(const std::string& userAgent){this->userAgent=userAgent;}

inline     std::vector<std::string> getHeaders() const { return this->headers; }
inline     void setHeaders(const std::vector<std::string>& headers) { this->headers = headers; }

inline HTTPREQUESTCALLBACK getCallback()const{return hcall;}
inline void setCallback(HTTPREQUESTCALLBACK hcall){this->hcall=hcall;}
std::string getDebugData()const{return debug_buffer.str();}

inline     HttpService* getService() const { return this->hservice; }
inline     void setService(HttpService* service) { this->hservice = service; }

    std::string toString() const;
    void cleanup();
    void release();
    void waitForCompletion();
void executeCallback();

private:
    friend class HttpService;
    friend class CurlHandler;
};

typedef std::shared_ptr<HttpRequest> shared_HttpRequest;
} // namespace gpp
#endif // HTTPREQUEST_H

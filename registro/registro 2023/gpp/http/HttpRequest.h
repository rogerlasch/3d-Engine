


#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

namespace gpp{
enum HTTPREQUEST_STATUS{
HTTPREQUEST_DEFAULT=0,
HTTPREQUEST_UNKNOWN,
HTTPREQUEST_INVALID_HANDLE,
HTTPREQUEST_ACTIVE,
HTTPREQUEST_ALREADY,
HTTPREQUEST_ERROR,
HTTPREQUEST_FINISHED,
};

class HttpRequest{
private:
uint32 id;
public:
int32 code;
std::string name;
std::string url;
std::string body;
std::string postFields;
std::string response;
std::string method;
std::vector<std::string> headers;
private:
CurlHandler* handle;
std::atomic<uint32> hstate;
static std::atomic<uint32> refcount;
public:
HttpRequest();
HttpRequest(const HttpRequest& http)=delete;
HttpRequest& operator=(const HttpRequest& http)=delete;
virtual ~HttpRequest();
void cleanup();
std::string toString();
uint32 getId()const;
uint32 getState()const;
uint32 start();
private:
static size_t http_push_memory_callback(void* ptr, size_t size, size_t nmemb, void *userp);
friend class HttpPool;
friend class HttpManager;
};
}
#endif

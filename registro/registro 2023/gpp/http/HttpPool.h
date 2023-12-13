


#ifndef HTTPPOOL_H
#define HTTPPOOL_H

namespace gpp{
class HttpPool{
private:
CURLM* mhandle;
mutable std::mutex mtx;
std::atomic<uint32> actives;
static std::atomic<uint32> refcount;
public:
HttpPool();
HttpPool(const HttpPool& hp)=delete;
HttpPool& operator=(const HttpPool& hp)=delete;
virtual ~HttpPool();
uint32 getActives()const;
void pushRequest(HttpRequest* req);
void removeRequest(HttpRequest* req);
void update(std::vector<uint32>& ids);
};
}
#endif

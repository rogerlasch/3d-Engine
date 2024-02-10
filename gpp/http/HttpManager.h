


#ifndef HTTPMANAGER_H
#define HTTPMANAGER_H

#include<vector>
#include<unordered_map>
#include<mutex>
#include<thread>
#include<condition_variable>
#include"HttpRequestStatus.h"

namespace gpp{

typedef std::function<void(uint32)> NOTIFICATION_HTTP_CALLBACK;

class HttpRequest;
class HttpPool;
class HttpManager{
private:
std::thread handle;
StateManager hstate;
HttpPool* hpool;
std::unordered_map<uint32, HttpRequest*> handles;
NOTIFICATION_HTTP_CALLBACK hnotify;
mutable std::mutex mtx;
mutable std::condition_variable hvar;
public:
HttpManager();
HttpManager(const HttpManager& hs)=delete;
HttpManager& operator=(const HttpManager& hs)=delete;
virtual ~HttpManager();

void setNotificationCallback(NOTIFICATION_HTTP_CALLBACK hcall);
NOTIFICATION_HTTP_CALLBACK getNotificationCallback()const;
void setName(uint32 id, const std::string& name);
std::string getName(uint32 id)const;

void start();
void stop();
uint32 count()const;
uint32 createRequest(const std::string& method, const std::string& url, const std::string& postFields="", const std::vector<std::string>& headers={});
uint32 getState(uint32 id)const;
uint32 getResponse(uint32 id, std::string& response);
uint32 wait(uint32 id, std::string& response);
private:
void onLoop();
};
}
#endif

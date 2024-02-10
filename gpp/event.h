

#ifndef GPP_EVENT_H
#define GPP_EVENT_H
#include <type_traits>

namespace gpp
{

enum GPP_EVENT_TYPES
{
GEVENT_DEFAULT=0,
GEVENT_CONNECTED,
GEVENT_DISCONNECTED,
GEVENT_RECEIVE,
GEVENT_PING,
GEVENT_KEYDOWN,
GEVENT_KEYUP,
GEVENT_TEXT,
GEVENT_CONNECT_SERVER,
GEVENT_DISCONNECT_SERVER,
};

class Event
{
public:
uint32 type;
uint32 v1;
uint32 v2;
uint32 v3;
int64 timestamp;
int64 timeout;
std::string data;
packet* pack;
void* userdata;
Event* next;
Event();
Event(const Event& ev)=delete;
Event& operator=(const Event& ev)=delete;
virtual ~Event();
void reset();
};

template<class T>
inline T* event_create_new(){
_GASSERT((std::is_base_of<Event, T>::value)==true);
return new T();
}
}
#endif

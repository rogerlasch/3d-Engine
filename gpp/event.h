

#ifndef GPP_EVENT_H
#define GPP_EVENT_H

namespace gpp
{

enum GPP_EVENT_TYPES
{
GEVENT_DEFAULT=0,
GEVENT_CONNECTED,
GEVENT_DISCONNECTED,
GEVENT_RECEIVE,
GEVENT_PING,
GEVENT_CONNECT_SERVER,
GEVENT_DISCONNECT_SERVER,
};

class Event
{
public:
uint32 peer_id;
uint32 type;
int32 code;
int64 timestamp;
int64 timeout;
std::string data;
packet* pack;
void* user;
Event* next;
Event();
Event(const Event& ev)=delete;
Event& operator=(const Event& ev)=delete;
virtual ~Event();
void reset();
};
}
#endif

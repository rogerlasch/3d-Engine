

#ifndef GPP_EVENT_H
#define GPP_EVENT_H

namespace gpp
{

enum GPP_EVENT_TYPES
{
GEVENT_DEFAULT=0,
GEVENT_RECEIVE,
GEVENT_PING
};

class event
{
public:
uint32 peer_id;
uint32 type;
int64 timestamp;
int64 timeout;
std::string data;
packet* pack;
event* next;
event();
event(const event& ev)=delete;
event& operator=(const event& ev)=delete;
virtual ~event();
void reset();
};
}
#endif

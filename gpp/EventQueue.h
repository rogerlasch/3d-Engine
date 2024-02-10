

#ifndef GPP_EVENTQUEUE_H
#define GPP_EVENTQUEUE_H

#include<functional>

namespace gpp
{

class EventQueue
{
private:
Event* m_first;
Event* m_last;
std::atomic<uint32> m_size;
mutable std::mutex mtx_queue;
public:
EventQueue();
EventQueue(const EventQueue& ev)=delete;
EventQueue& operator=(const EventQueue& ev)=delete;
virtual ~EventQueue();
uint32 size()const;
bool empty()const;
void eventDestroyAll();
bool eventGet(Event** ev);
void eventPost(uint32 type, uint32 v1=0, uint32 v2=0, uint32 v3=0, int64 timeout=0, packet* pack=NULL);
void eventPost(uint32 type, const std::string& data, void* user);
void eventPost(Event* ev);
};
typedef std::function<void(Event*)> EVENTPOSTCALLBACK;
}
#endif

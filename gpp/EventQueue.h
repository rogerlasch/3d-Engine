

#ifndef GPP_EVENTQUEUE_H
#define GPP_EVENTQUEUE_H

namespace gpp
{

class EventQueue
{
private:
event* m_first;
event* m_last;
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
bool eventGet(event** ev);
void eventPost(uint32 peer_id, uint32 type, int64 timeout=0, packet* pack=NULL);
void eventPost(event* ev);
};
}
#endif

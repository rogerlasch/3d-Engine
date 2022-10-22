


#include<atomic>
#include<string>
#include<mutex>
#include"types.h"
#include"debug.h"
#include"packet.h"
#include"event.h"
#include"EventQueue.h"

using namespace std;

namespace gpp
{
EventQueue::EventQueue()
{
m_first=NULL;
m_last=NULL;
m_size.store(0);
}

EventQueue::~EventQueue()
{
if(m_size.load()>0)
{
eventDestroyAll();
}
}

uint32 EventQueue::size()const
{
return this->m_size.load();
}

bool EventQueue::empty()const
{
return this->m_size.load()==0;
}

void EventQueue::eventDestroyAll()
{
lock_guard<mutex> lck(this->mtx_queue);
if(m_size.load()==0)
{
m_first=NULL;
m_last=NULL;
return;
}
while(m_first!=NULL)
{
event* ev=m_first->next;
m_first=m_first->next;
delete ev;
}
m_last=NULL;
m_size.store(0);
}

bool EventQueue::eventGet(event** ev)
{
if((ev==NULL)||(m_size.load()==0))
{
return false;
}
lock_guard<mutex> lck(mtx_queue);
*ev=m_first;
m_first=m_first->next;
m_size.fetch_sub(1);
if(m_size.load()==0)
{
m_first=NULL;
m_last=NULL;
}
return true;
}

void EventQueue::eventPost(uint32 peer_id, uint32 type, int64 timeout, packet* pack)
{
event* ev=new event();
ev->peer_id=peer_id;
ev->type=type;
ev->timeout=timeout;
ev->pack=pack;
eventPost(ev);
}

void EventQueue::eventPost(event* ev)
{
if(ev==NULL)
{
return;
}
lock_guard<mutex> lck(mtx_queue);
if(m_size.load()==0)
{
m_first=ev;
m_last=ev;
m_first->next=NULL;
m_last->next=NULL;
m_size.fetch_add(1);
}
else
{
m_last->next=ev;
m_last=m_last->next;
m_size.fetch_add(1);
}
}
}

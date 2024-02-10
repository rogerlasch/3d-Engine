


#include<atomic>
#include<string>
#include<mutex>
#include"debug_system.h"
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
Event* ev=m_first->next;
m_first=m_first->next;
delete ev;
}
m_last=NULL;
m_size.store(0);
}

bool EventQueue::eventGet(Event** ev)
{
if((ev==NULL)||(m_size.load()==0))
{
return false;
}
try{
lock_guard<mutex> lck(mtx_queue);
*ev=m_first;
m_first=m_first->next;
(*ev)->next=NULL;
m_size.fetch_sub(1);
if(m_size.load()==0)
{
m_first=NULL;
m_last=NULL;
}
return true;
}catch(const exception& e){
_GCRITICAL("{}", _GEXCEPT(e.what()).what());
return false;
}
return false;
}

void EventQueue::eventPost(uint32 type, uint32 v1, uint32 v2, uint32 v3, int64 timeout, packet* pack){
Event* ev=new Event();
ev->type=type;
ev->v1=v1;
ev->v2=v2;
ev->v3=v3;
ev->timeout=timeout;
ev->pack=pack;
eventPost(ev);
}

void EventQueue::eventPost(uint32 type, const string& data, void* user){
Event* ev=new Event();
ev->type=type;
ev->data=data;
ev->userdata = user;
ev->timeout=0;
eventPost(ev);
}

void EventQueue::eventPost(Event* ev)
{
if(ev==NULL)
{
return;
}
try{
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
}catch(const exception& e){
_GCRITICAL("{}", _GEXCEPT(e.what()).what());
}
}
}

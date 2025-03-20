

#ifndef EVENTFRAME_H
#define EVENTFRAME_H

#include<functional>
#include"../types.h"
namespace gpp{

class EventFrame;
typedef std::function<void(EventFrame*)> EVENTFRAMECALLBACK;
class EventFrame{
public:
uint32 id;
uint32 counter;
uint32 frequence;
void* userdata;
void* v1;
void* v2;
EVENTFRAMECALLBACK hcall;
bool oneFire;
public:
EventFrame();
EventFrame(const EventFrame& ev)=delete;
EventFrame& operator=(const EventFrame& ev)=delete;
virtual ~EventFrame()=default;

void cleanup();
EventFrame* clone();
};
}
#endif

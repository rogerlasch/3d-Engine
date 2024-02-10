

#include<string>
#include<mutex>
#include"debug_system.h"
#include"packet.h"
#include"event.h"

using namespace std;

namespace gpp
{
static mutex mtx;

Event::Event()
{
pack=NULL;
next=NULL;
userdata=NULL;
this->reset();
}

Event::~Event()
{
this->reset();
}

void Event::reset()
{
type=GEVENT_DEFAULT;
v1=0;
v2=0;
v3=0;
timestamp=get_timestamp_ms();
timeout=0;
data="";
userdata=NULL;
next=NULL;
if(pack!=NULL)
{
delete pack;
}
pack=NULL;
}
}

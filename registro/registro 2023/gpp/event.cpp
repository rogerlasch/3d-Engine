

#include<string>
#include"debug_system.h"
#include"packet.h"
#include"event.h"

using namespace std;

namespace gpp
{
Event::Event()
{
pack=NULL;
next=NULL;
user=NULL;
this->reset();
}

Event::~Event()
{
this->reset();
}

void Event::reset()
{
type=GEVENT_DEFAULT;
peer_id=0;
code=0;
timestamp=get_timestamp_ms();
timeout=0;
data="";
user=NULL;
next=NULL;
if(pack!=NULL)
{
delete pack;
}
pack=NULL;
}
}

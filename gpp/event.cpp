

#include<string>
#include"types.h"
#include"debug.h"
#include"profiler.h"
#include"time.h"
#include"packet.h"
#include"event.h"

using namespace std;

namespace gpp
{
event::event()
{
profiler_snap();
pack=NULL;
next=NULL;
this->reset();
}

event::~event()
{
profiler_snap();
this->reset();
}

void event::reset()
{
type=GEVENT_DEFAULT;
peer_id=0;
timestamp=get_timestamp_ms();
timeout=0;
data="";
next=NULL;
if(pack!=NULL)
{
delete pack;
}
pack=NULL;
}
}

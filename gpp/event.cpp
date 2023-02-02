

#include<string>
#include"types.h"
#include"debug.h"
#include"time.h"
#include"packet.h"
#include"event.h"

using namespace std;

namespace gpp
{
Event::Event()
{
pack=NULL;
next=NULL;
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

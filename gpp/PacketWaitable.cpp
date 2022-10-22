

#include<atomic>
#include<mutex>
#include<shared_mutex>
#include"types.h"
#include"debug.h"
#include"time.h"
#include"SafeFlags.h"
#include"packet.h"
#include"PacketWaitable.h"

using namespace std;

namespace gpp
{
PacketWaitable::PacketWaitable():
SafeFlags(0)
{
psend=NULL;
presult=NULL;
timestamp.store(0);
timeout.store(0);
}

PacketWaitable::~PacketWaitable()
{
}

void PacketWaitable::cleanup()
{
try {
lock_guard<mutex> lck(mtx);
psend=NULL;
presult=NULL;
timestamp.store(0);
timeout.store(0);
replace_flags(0);
} catch(const exception& e) {
_LOG_EXCEPT(e.what());
}
}

bool PacketWaitable::isWaiting()const
{
return flag_contains(PWF_WAITING);
}

bool PacketWaitable::isAnswer(packet* ps)
{
lock_guard<mutex> lck(mtx);
if((this->flag_contains(PWF_READ))||(psend==NULL)||(ps==NULL))
{
return false;
}
if((ps->type==PACKET_ANSWER_WAITABLE)&&(ps->subtype==psend->subtype)&&(ps->command==psend->command))
{
presult=ps;
this->setflag(PWF_READ);
return true;
}
return false;
}
}


#include <GameNetworkingSockets/steam/steamnetworkingsockets.h>
#include <GameNetworkingSockets/steam/isteamnetworkingutils.h>
#include<atomic>
#include<functional>
#include<memory>
#include<mutex>
#include<shared_mutex>
#include<string>
#include<unordered_map>
#include"types.h"
#include"debug.h"
#include"profiler.h"
#include"time.h"
#include"SafeFlags.h"
#include"packet.h"
#include"PacketWaitable.h"
#include"basic_peer.h"
#include"event.h"
#include"EventQueue.h"
#include"GppSteamBase.h"
#include"GppClientServerBase.h"

using namespace std;

namespace gpp
{
GppClientServerBase::GppClientServerBase(uint32 htype):
GppSteamBase(), EventQueue()
{
this->htype=htype;
}

GppClientServerBase:: ~GppClientServerBase()
{
}

uint32 GppClientServerBase::getHtype()const
{
return this->htype;
}

bool GppClientServerBase::isClient()const
{
return this->htype==GNET_CLIENT;
}

bool GppClientServerBase::isServer()const
{
return this->htype==GNET_SERVER;
}

void GppClientServerBase::run()
{
}
}

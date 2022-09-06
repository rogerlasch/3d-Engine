

#include <GameNetworkingSockets/steam/steamnetworkingsockets.h>
#include <GameNetworkingSockets/steam/isteamnetworkingutils.h>
#include<mutex>
#include<string>
#include"types.h"
#include"debug.h"
#include"debug.h"
#include"GppSteamBase.h"


using namespace std;

namespace gpp
{

GppSteamBase* GppSteamBase::hinstance=NULL;
mutex GppSteamBase::mtx_hinstance;
atomic<uint32> GppSteamBase::steamrefcount=0;
GppSteamBase::GppSteamBase()
{
steamrefcount.fetch_add(1);
if(steamrefcount.load()==1)
{
SteamDatagramErrMsg error;
if(!GameNetworkingSockets_Init(NULL, error))
{
throw runtime_error("Erro ao inicializar a GameNetworkingSockets.");
}
}
hsteam=SteamNetworkingSockets();
}

GppSteamBase::~GppSteamBase()
{
if(steamrefcount.load()>0)
{
steamrefcount.fetch_sub(1);
if(steamrefcount.load()==0)
{
GameNetworkingSockets_Kill();
}
}
}

void GppSteamBase::safeRunCallbacks()
{
lock_guard<mutex> lck(this->mtx_hinstance);
if(hsteam!=NULL)
{
hinstance=this;
hsteam->RunCallbacks();
hinstance=NULL;
}
}

void GppSteamBase::connectionStatusCallback(SteamNetConnectionStatusChangedCallback_t* pinfo)
{
_FLOG("O método {} não foi sobrescrito corretamente!", __FUNCTION__);
}

void GppSteamBase::staticConnectionStatusCallback(SteamNetConnectionStatusChangedCallback_t* pinfo)
{
if(hinstance!=NULL)
{
hinstance->connectionStatusCallback(pinfo);
}
}
}

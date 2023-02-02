
#include"gpp.h"
#include"gpp_steamsockets.h"

using namespace std;

namespace gpp
{
ISteamNetworkingSockets* hsteam=NULL;
mutex gpp_steamsockets::mtx_hinstance;
atomic<uint32> gpp_steamsockets::refcount=0;
gpp_steamsockets* gpp_steamsockets::hinstance=NULL;
gpp_steamsockets::gpp_steamsockets()
{
this->refcount.fetch_add(1);
if(refcount.load()==1)
{
SteamDatagramErrMsg error;
if(!GameNetworkingSockets_Init(NULL, error))
{
throw runtime_error("Erro ao inicializar a GameNetworkingSockets.");
}
hsteam=SteamNetworkingSockets();
}
}

gpp_steamsockets:: ~gpp_steamsockets()
{
if(refcount.load()>0)
{
refcount.fetch_sub(1);
if(refcount.load()==0)
{
GameNetworkingSockets_Kill();
hsteam=NULL;
}
}
}

 uint32 gpp_steamsockets::connectToServer(const string& address, uint16 port, GPPCONNECTIONCALLBACK hcall)
{
if((address.size()==0)||(port==0))
{
return 0;
}
SteamNetworkingIPAddr serverAddress;
serverAddress.Clear();
if(!serverAddress.ParseString(address.c_str()))
{
return 0;
}
serverAddress.m_port=port;
SteamNetworkingConfigValue_t opt;
opt.SetPtr( k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, staticConnectionStatusCallback);
uint32 sock=hsteam->ConnectByIPAddress( serverAddress, 1, &opt );
if(sock==k_HSteamNetConnection_Invalid )
{
return 0;
}
this->setConnectionCallBack(hcall);
return sock;
}

uint32 gpp_steamsockets::createListenSocket(uint16 port, GPPCONNECTIONCALLBACK hcall)
{
_GASSERT_MSG(hsteam!=NULL, "A isteamnetworkingsockets não foi inicializada!");
_GASSERT(hsteam!=NULL);
SteamNetworkingIPAddr localAddress;
localAddress.Clear();
localAddress.m_port=port;
SteamNetworkingConfigValue_t opt;
opt.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, staticConnectionStatusCallback);
int32 listensock=hsteam->CreateListenSocketIP(localAddress, 1, &opt );
if(listensock==k_HSteamListenSocket_Invalid)
{
return 0;
}
this->setConnectionCallBack(hcall);
return listensock;
}

uint32 gpp_steamsockets::closeListenSocket(uint32 sock)
{
_GASSERT_MSG(hsteam!=NULL, "A isteamnetworkingsockets não foi inicializada!");
//profiler_snap();
return ((hsteam->CloseListenSocket(sock)) ? 1 : 0);
}

 uint32 gpp_steamsockets::sendReliable(uint32 peer_id, const GMESSAGE& msg)
{
_GASSERT_MSG(hsteam!=NULL, "A isteamnetworkingsockets não foi inicializada!");
//profiler_snap();
_GASSERT(hsteam!=NULL);
int32 res=hsteam->SendMessageToConnection(peer_id, msg.c_str(), msg.size(), k_nSteamNetworkingSend_Reliable, NULL);
return res==k_EResultOK;
}

 uint32 gpp_steamsockets::sendUnreliable(uint32 peer_id, const GMESSAGE& msg)
{
_GASSERT_MSG(hsteam!=NULL, "A isteamnetworkingsockets não foi inicializada!");
//profiler_snap();
_GASSERT(hsteam!=NULL);
int32 res=hsteam->SendMessageToConnection(peer_id, msg.c_str(), msg.size(), k_nSteamNetworkingSend_Unreliable, NULL);
return res==k_EResultOK;
}

 uint32 gpp_steamsockets::receiveMessage(uint32 peer_id, GMESSAGE& msg)
{
_GASSERT_MSG(hsteam!=NULL, "A isteamnetworkingsockets não foi inicializada!");
//profiler_snap();
_GASSERT(hsteam!=NULL);
SteamNetworkingMessage_t * msgs=NULL;
int32 size=0;
if((size=hsteam->ReceiveMessagesOnConnection(peer_id, &msgs, 1))<=0)
{
return 0;
}
msg.assign((char*)msgs[0].m_pData, msgs[0].m_cbSize);
msgs[0].Release();
return msg.size();
}

 uint32 gpp_steamsockets::receiveMessages(uint32 peer_id, GMESSAGELIST& msgs, uint32 n)
{
_GASSERT_MSG(hsteam!=NULL, "A isteamnetworkingsockets não foi inicializada!");
//profiler_snap();
msgs.clear();
msgs.reserve(n+1);
int32 size=0;
SteamNetworkingMessage_t * mpool=NULL;
if((size=hsteam->ReceiveMessagesOnConnection(peer_id, &mpool, n))<=0)
{
return 0;
}
for(int i=0; i<size; i++)
{
msgs.emplace_back((char*)mpool[i].m_pData, mpool[i].m_cbSize);
mpool[i].Release();
}
return size;
}

 bool gpp_steamsockets::disconnectPeer(uint32 peer_id, uint32 gmode)
{
_GASSERT_MSG(hsteam!=NULL, "A isteamnetworkingsockets não foi inicializada!");
//profiler_snap();
switch(gmode)
{
case GMODE_NOW:
{
return hsteam->CloseConnection(peer_id, 0, NULL, false);
}
case GMODE_SOFTLY:
{
return hsteam->CloseConnection(peer_id, 0, NULL, true);
}
}
return false;
}

void gpp_steamsockets::onLoop()
{
this->safeRunCallbacks();
}

void gpp_steamsockets::safeRunCallbacks()
{
_GASSERT_MSG(hsteam!=NULL, "A isteamnetworkingsockets não foi inicializada!");
lock_guard<mutex> lck(this->mtx_hinstance);
if(hsteam!=NULL)
{
hinstance=this;
hsteam->RunCallbacks();
hinstance=NULL;
}
}

void gpp_steamsockets::connectionStatusCallback(SteamNetConnectionStatusChangedCallback_t* pinfo)
{
_GASSERT_MSG(hcall!=NULL, "O retorno de chamada não foi definido corretamente.");
_GASSERT_MSG(hsteam!=NULL, "A isteamnetworkingsockets não foi inicializada!");
switch(pinfo->m_info.m_eState)
{
case k_ESteamNetworkingConnectionState_Connecting:
{
int32 res=hsteam->AcceptConnection(pinfo->m_hConn);
if(res!=k_EResultOK)
{
//hsteam->CloseConnection( pinfo->m_hConn, 0, nullptr, false );
break;
}
hcall(GEVENT_CONNECTED, pinfo->m_hConn );
break;
}
                        case k_ESteamNetworkingConnectionState_ClosedByPeer:
                        case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
                        {
                                if ( pinfo->m_eOldState == k_ESteamNetworkingConnectionState_Connected )
                                {
hcall(GEVENT_DISCONNECTED, pinfo->m_hConn );
hsteam->CloseConnection(pinfo->m_hConn, 0, NULL, false);
}
else
{
hcall(GEVENT_DISCONNECTED, pinfo->m_hConn );
}
break;
                        }
default:
{
break;
}
}
}

void gpp_steamsockets::staticConnectionStatusCallback(SteamNetConnectionStatusChangedCallback_t* pinfo)
{
if(hinstance!=NULL)
{
hinstance->connectionStatusCallback(pinfo);
}
}
}

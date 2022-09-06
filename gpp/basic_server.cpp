

#include <GameNetworkingSockets/steam/steamnetworkingsockets.h>
#include <GameNetworkingSockets/steam/isteamnetworkingutils.h>
#include<atomic>
#include<functional>
#include<mutex>
#include<shared_mutex>
#include<string>
#include<unordered_map>
#include"types.h"
#include"debug.h"
#include"basic_peer.h"
#include"GppSteamBase.h"
#include"basic_server.h"

using namespace std;

namespace gpp
{

basic_server::basic_server():
GppSteamBase()
{
hstate.store(SERVER_DEFAULT);
listensock=0;
}

basic_server::~basic_server()
{
if(hstate.load()==SERVER_RUNNING)
{
this->shutdown();
}
}

uint32 basic_server::getPort()const
{
return this->listenport;
}

uint32 basic_server::getMaxPlayers()const
{
return this->max_players;
}

uint32 basic_server::getHstate()const
{
return hstate.load();
}

bool basic_server::start(uint32 port, uint32 max_players)
{
if(hstate.load()!=SERVER_DEFAULT)
{
return false;
}
if((port<1)||(port>65535)||(max_players==0)||(max_players>30000))
{
return false;
}
hstate.store(SERVER_STARTING);
SteamNetworkingIPAddr localAddress;
localAddress.Clear();
localAddress.m_port=port;
SteamNetworkingConfigValue_t opt;
opt.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, staticConnectionStatusCallback);
listensock=hsteam->CreateListenSocketIP(localAddress, 1, &opt );
if(listensock==k_HSteamListenSocket_Invalid)
{
hstate.store(SERVER_DEFAULT);
return false;
}
this->listenport=port;
this->max_players=max_players;
hstate.store(SERVER_RUNNING);
return true;
}

bool basic_server::shutdown()
{
switch(hstate.load())
{
case SERVER_STARTING:
case SERVER_RUNNING:
{
hstate.store(SERVER_SHUTING_DOWN);
peerDisconnectAll();
hsteam->CloseListenSocket(listensock);
listensock=k_HSteamListenSocket_Invalid;
hsteam=NULL;
break;
}
}
return true;
}

bool basic_server::run()
{
if(hstate.load()!=SERVER_RUNNING)
{
return false;
}
while(hstate.load()==SERVER_RUNNING)
{
this_thread::sleep_for(chrono::milliseconds(5));
this->update();
}
return true;
}

void basic_server::update()
{
//Primeiro passo,  verificar os sockets e tratar as conexões...
shared_ptr<unique_lock<shared_mutex>> s_lock=make_shared<unique_lock<shared_mutex>>(mtx_con);
for(auto it=connections.begin(); it!=connections.end(); ++it)
{
it->second->updateCon();
}
s_lock.reset();
safeRunCallbacks();
}

bool basic_server::peerDisconnect(uint32 peer_id)
{
auto it=connections.find(peer_id);
if(it==connections.end())
{
return false;
}
if((it->second->isConnected())&&(it->second->disconnect()))
{
it->second->setHstate(PEER_DISCONNECTING);
return true;
}
return false;
}

bool basic_server::peerDisconnectNow(uint32 peer_id)
{
auto it=connections.find(peer_id);
if(it==connections.end())
{
return false;
}
it->second->disconnectNow();
delete it->second;
connections.erase(it);
return true;
}

void basic_server::peerDisconnectAll()
{
for(auto it=connections.begin(); it!=connections.end(); ++it)
{
it->second->disconnect();
}
}

bool basic_server::peerIsConnected(uint32 peer_id)const
{
return connections.find(peer_id)!=connections.end();
}

void basic_server::connectionStatusCallback(SteamNetConnectionStatusChangedCallback_t* pinfo)
{
switch(pinfo->m_info.m_eState)
{
case k_ESteamNetworkingConnectionState_Connecting:
{
_FLOG("Connection request from {}", pinfo->m_info.m_szConnectionDescription);
int32 res=hsteam->AcceptConnection(pinfo->m_hConn);
if(res!=k_EResultOK)
{
//hsteam->CloseConnection( pinfo->m_hConn, 0, nullptr, false );
break;
}
_FLOG("Alguém se conectou!");
basic_peer* c=new basic_peer();
c->setPeerId(pinfo->m_hConn );
c->setHstate(PEER_ALT);
c->setHserver(hsteam);
connections.insert(make_pair(c->getPeerId(), c));
break;
}
                        case k_ESteamNetworkingConnectionState_ClosedByPeer:
                        case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
                        {
                                if ( pinfo->m_eOldState == k_ESteamNetworkingConnectionState_Connected )
                                {
auto it=connections.find( pinfo->m_hConn );
if(it==connections.end())
{
return;
}
delete it->second;
connections.erase(it);
hsteam->CloseConnection( pinfo->m_hConn, 0, nullptr, false );
}
                                break;
                        }
default:
{
break;
}
}
}
}

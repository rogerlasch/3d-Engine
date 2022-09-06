

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
#include"basic_client.h"

using namespace std;

namespace gpp
{

basic_client::basic_client():
basic_peer(), GppSteamBase()
{
port=0;
ipaddress="";
this->setHserver(this->hsteam);
}

basic_client::~basic_client()
{
}

uint32 basic_client::getPort()const
{
return this->port;
}

string basic_client::getAddress()const
{
return this->ipaddress;
}

bool basic_client::connect(const string& address, uint32 port)
{
if((address.size()==0)||(port==0)||(port>65535)||(this->getHstate()!=PEER_DEFAULT))
{
return false;
}
SteamNetworkingIPAddr serverAddress;
setHserver(SteamNetworkingSockets());
serverAddress.Clear();
if(!serverAddress.ParseString(address.c_str()))
{
return false;
}
serverAddress.m_port=port;
SteamNetworkingConfigValue_t opt;
opt.SetPtr( k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, staticConnectionStatusCallback);
uint32 sock=hserver->ConnectByIPAddress( serverAddress, 1, &opt );
if(sock==k_HSteamNetConnection_Invalid )
{
return false;
}
this->port=port;
this->ipaddress=address;
this->setPeerId(sock);
hstate.store(PEER_CONNECTED);
return true;
}

void basic_client::run()
{
while(hstate.load()==PEER_CONNECTED)
{
this_thread::sleep_for(chrono::milliseconds(5));
this->update();
}
}

void basic_client::update()
{
basic_peer::updateCon();
this->safeRunCallbacks();
}

//Private metods...

void basic_client::connectionStatusCallback(SteamNetConnectionStatusChangedCallback_t* pinfo)
{
                switch(pinfo->m_info.m_eState )
                {
                        case k_ESteamNetworkingConnectionState_None:
{
// NOTA: Receberemos retornos de chamada aqui quando destruirmos as conexões. Você pode ignorá-los.
                                break;
}
                        case k_ESteamNetworkingConnectionState_ClosedByPeer:
                        case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
                        {
this->setHstate(PEER_DISCONNECTED);
this->setPeerId(0);
hserver->CloseConnection( pinfo->m_hConn, 0, nullptr, false );
                                break;
                        }
                        case k_ESteamNetworkingConnectionState_Connecting:
                        {
// Esta deve ser uma nova conexão
// Um cliente está tentando se conectar
// Tenta aceitar a conexão.
                                break;
                        }
                        case k_ESteamNetworkingConnectionState_Connected:
{
// Receberemos um retorno de chamada imediatamente após aceitar a conexão.
// Como somos o servidor, podemos ignorar isso, não é novidade para nós.
                                break;
}
                        default:
{
// Silencia -Wswitch
                                break;
}
                }
}
}

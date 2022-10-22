

/**
*Escrito por Roger em setembro de 2022.
*Este arquivo faz parte da game programming platform.
**/
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
#include"event.h"
#include"EventQueue.h"
#include"GppSteamBase.h"
#include"GppClientServerBase.h"
#include"basic_peer.h"
#include"basic_server.h"

using namespace std;

namespace gpp
{

basic_server::basic_server():
GppClientServerBase(GNET_SERVER)
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

uint32 basic_server::playerCount()const
{
shared_lock<shared_mutex> lck(mtx_con);
return connections.size();
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

/**
*Entra em loop enquanto a conexão estiver aberta.
*Todo o processo de loop é feito aqui, como ler o socket do servidor, processar eventos, etc...
*Isto é útil para deixar em um thread secundário...
**/
void basic_server::run()
{
profiler_snap();
while(hstate.load()==PEER_CONNECTED)
{
wait_ms(5);
this->update();
}
}

void basic_server::update()
{
pollNet();
pollEvents();
}

/**
*Faça tudo o que for relacionado a internet aqui...
**/
void basic_server::pollNet()
{
profiler_snap();
this->safeRunCallbacks();
for(auto it=connections.begin(); it!=connections.end(); ++it)
{
switch(it->second->getHstate())
{
case PEER_ALT:
case PEER_CONNECTED:
case PEER_DISCONNECTING:
{
SteamNetworkingMessage_t * msgs=NULL;
uint32 max_messages=10;
int32 size=0;
hsteam->FlushMessagesOnConnection(it->first);
//Tente ler algumas mensagens da conexão...
if((size=hsteam->ReceiveMessagesOnConnection(it->first, &msgs, max_messages))<=0)
{
return;
}
for(int32 i=0; i<size; i++)
{
string data="";
data.assign((char*)msgs[i].m_pData, msgs[i].m_cbSize);
msgs[i].Release();
processPackagedNetMessage(it->second, data);
}
break;
}
}
}
}

void basic_server::pollEvents()
{
event* ev=NULL;
while(eventGet(&ev))
{
dispatchEvent(ev);
delete ev;
}
}

 void basic_server::processSimpleNetMessage(basic_peer* peer, const string& msg)
{
_FLOG("Processando uma mensagem sem empacotamento... \"{}\"", msg);
}

 void basic_server::processPackagedNetMessage(basic_peer* peer, const string& msg)
{
packet* pack=new packet();
if(!pack->deserialize(msg))
{
delete pack;
_FLOG("Erro ao deserializar o pacote \"{}\"", msg);
return;
}
//Analise o pacote recebido...
switch(pack->type)
{
//Apenas responda rapidamente...
case PACKET_PING:
{
peer->sendPacketReliable(PACKET_PONG, 0, 0, "");
delete pack;
break;
}
//O servidor recebeu um pacote waitable...
case PACKET_WAITABLE:
{
peer->hpack->psend=pack;
event* ev=new event();
ev->type=GEVENT_RECEIVE;
ev->peer_id=peer->getPeerId();
ev->pack=pack;
eventPost(ev);
break;
}
//Caso seja um outro tipo de pacote, gere o evento e despache ele para a fila de eventos.
default:
{
event* ev=new event();
ev->type=GEVENT_RECEIVE;
ev->peer_id=peer->getPeerId();
ev->pack=pack;
eventPost(ev);
break;
}
}
}

/**
*Método para tratar os eventos recuperados da fila de eventos.
*Isto pode ser basicamente, qualquer coisa.
**/
void basic_server::dispatchEvent(event* ev)
{
if(ev==NULL)
{
return;
}
switch(ev->type)
{
case GEVENT_PING:
{
_FLOG("O ping foi de {} ms.", ev->data);
break;
}
case GEVENT_RECEIVE:
{
basic_peer* ch=connections.at(ev->peer_id);
ch->sendAnswerSuccess("Sua mensagem foi recebida com sucesso.");
break;
}
}
}

bool basic_server::peerDisconnect(uint32 peer_id)
{
profiler_snap();
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
profiler_snap();
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
profiler_snap();
if(playerCount()==0)
{
return;
}
unique_lock<shared_mutex> lck(mtx_con);
for(auto it=connections.begin(); it!=connections.end(); ++it)
{
it->second->disconnect();
}
lck.unlock();
int64 start=get_timestamp_ms();
bool timeout=false;
while(playerCount()>0)
{
wait_ms(10);
pollNet();
if((get_timestamp_ms()-start)>2000)
{
timeout=true;
break;
}
}
if(timeout)
{
_FLOG("As conexões estão demorando de mais para serem finalizadas. Elas serão desconectadas a força agora...");
lck.lock();
for(auto it=connections.begin(); it!=connections.end(); ++it)
{
it->second->disconnectNow();
delete it->second;
}
}
connections.clear();
}

bool basic_server::peerIsConnected(uint32 peer_id)const
{
shared_lock<shared_mutex> lck(mtx_con);
return connections.find(peer_id)!=connections.end();
}

void basic_server::connectionStatusCallback(SteamNetConnectionStatusChangedCallback_t* pinfo)
{
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
_FLOG("Alguém se conectou!");
basic_peer* c=new basic_peer(PEER_REGULAR_CLIENT, GNET_SERVER);
c->setPeerId(pinfo->m_hConn );
c->setHstate(PEER_ALT);
c->setConnectionTime(get_timestamp_ms());
c->setHserver(hsteam);
unique_lock<shared_mutex> lck(mtx_con);
connections.insert(make_pair(c->getPeerId(), c));
break;
}
                        case k_ESteamNetworkingConnectionState_ClosedByPeer:
                        case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
                        {
                                if ( pinfo->m_eOldState == k_ESteamNetworkingConnectionState_Connected )
                                {
unique_lock<shared_mutex> lck(mtx_con);
auto it=connections.find( pinfo->m_hConn );
if(it==connections.end())
{
return;
}
delete it->second;
connections.erase(it);
hsteam->CloseConnection( pinfo->m_hConn, 0, nullptr, false );
_FLOG("Conexão fechada.");
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

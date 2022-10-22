
/**
*Escrito por Roger em setembro de 2022.
*Este arquivo faz parte da game programming platform.
*Classe de cliente básico. Será responsável por toda a comunicação com o servidor.
*Futuramente, os bots poderão herdar daqui e só implementarem sua IA.
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
#include"basic_client.h"

using namespace std;

namespace gpp
{

basic_client::basic_client(uint32 ctype):
GppClientServerBase(GNET_CLIENT), basic_peer(ctype, GNET_CLIENT)
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

/**
*Se conecta ao servidor e faz a altenticação.
*Caso a altenticação falhe, a conexão também falhará.
*Retorna falso se já estiver conectado...
**/
bool basic_client::connect(const string& address, uint32 port)
{
profiler_snap();
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

/**
*Entra em loop enquanto a conexão estiver aberta.
*Todo o processo de loop é feito aqui, como ler o socket do servidor, processar eventos, etc...
*Isto é útil para deixar em um thread secundário...
**/
void basic_client::run()
{
profiler_snap();
while(hstate.load()==PEER_CONNECTED)
{
wait_ms(5);
this->update();
}
}

void basic_client::update()
{
pollNet();
pollEvents();
}

/**
*Faça tudo o que for relacionado a internet aqui...
**/
void basic_client::pollNet()
{
profiler_snap();
//basic_peer::updateCon();
switch(getHstate())
{
case PEER_ALT:
case PEER_CONNECTED:
case PEER_DISCONNECTING:
{
this->safeRunCallbacks();
unique_lock<shared_mutex> lck(mtx_con);
SteamNetworkingMessage_t * msgs=NULL;
uint32 max_messages=10;
int32 size=0;
hserver->FlushMessagesOnConnection(peer_id);
//Tente ler algumas mensagens da conexão...
if((size=hserver->ReceiveMessagesOnConnection(peer_id, &msgs, max_messages))<=0)
{
return;
}
lck.unlock();
for(int32 i=0; i<size; i++)
{
string data="";
data.assign((char*)msgs[i].m_pData, msgs[i].m_cbSize);
msgs[i].Release();
processPackagedNetMessage((basic_peer*)this, data);
}
break;
}
}
}

void basic_client::pollEvents()
{
event* ev=NULL;
while(eventGet(&ev))
{
dispatchEvent(ev);
delete ev;
}
}

 void basic_client::processSimpleNetMessage(basic_peer* peer, const string& msg)
{
_FLOG("Processando uma mensagem sem empacotamento... \"{}\"", msg);
}

 void basic_client::processPackagedNetMessage(basic_peer* peer, const string& msg)
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
sendPacketReliable(PACKET_PONG, 0, 0, "");
delete pack;
break;
}
//Calcule o ping...
case PACKET_PONG:
{
if(onping.load()!=0)
{
int64 end=get_timestamp_ms();
event* ev=new event;
ev->type=GEVENT_PING;
ev->data=to_string(end-onping.load());
eventPost(ev);
onping.store(0);
}
delete pack;
break;
}
//O cliente recebeu uma resposta!
case PACKET_ANSWER_WAITABLE:
{
if((hpack->isWaiting())&&(hpack->isAnswer(pack)))
{
break;
}
else
{
_FLOG("O pacote de resposta chegou tarde de mais.");
delete pack;
}
break;
}
//Caso seja um outro tipo de pacote, gere o evento e despache ele para a fila de eventos.
default:
{
event* ev=new event();
ev->type=GEVENT_RECEIVE;
ev->peer_id=peer_id;
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
void basic_client::dispatchEvent(event* ev)
{
profiler_snap();
if(ev==NULL)
{
return;
}
switch(ev->type)
{
//O servidor respondeu ao pacote ping, então aqui está o resultado...
case GEVENT_PING:
{
_FLOG("O ping foi de {} ms.", ev->data);
break;
}
}
}

//Private metods...

/**
*Autoriza a conexão que está sendo estabelecida...
*O cliente gera um pacote contendo nome, chave de acesso e uma flag indicando se a conexão está sendo gerenciada por um bot.
*Caso o servidor responda positivamente, então a conexão é efetivada.
*Isto é feito toda vez que o método connect é chamado. Não deve ser invocado mais veses.
**/
bool basic_client::altConnection()
{
profiler_snap();
if(getHstate()!=PEER_ALT)
{
return false;
}
uint32 x=getCtype();
if((x!=PEER_BOT)&&(x!=PEER_REGULAR_CLIENT))
{
return false;
}
packet pack;
return true;
}

/**
*Callback para mudança de estados na conexão.
*Isto é utilizado pela biblioteca de rede da steam.
**/
void basic_client::connectionStatusCallback(SteamNetConnectionStatusChangedCallback_t* pinfo)
{
                switch(pinfo->m_info.m_eState )
                {
                        case k_ESteamNetworkingConnectionState_ClosedByPeer:
                        case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
                        {
this->setHstate(PEER_DISCONNECTED);
this->setPeerId(0);
hserver->CloseConnection( pinfo->m_hConn, 0, nullptr, true);
                                break;
                        }
                        case k_ESteamNetworkingConnectionState_Connected:
{
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

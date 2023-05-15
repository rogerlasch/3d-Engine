
/**
*Escrito por Roger em setembro de 2022.
*Este arquivo faz parte da game programming platform.
*Classe de cliente básico. Será responsável por toda a comunicação com o servidor.
*Futuramente, os bots poderão herdar daqui e só implementarem sua IA.
**/
#include<atomic>
#include<functional>
#include<memory>
#include<mutex>
#include<shared_mutex>
#include<string>
#include<unordered_map>
#include"debug_system.h"
#include"SafeFlags.h"
#include"packet.h"
#include"event.h"
#include"EventQueue.h"
#include"gpp_networkinterface.h"
#include"gpp_peer.h"
#include"gpp_client.h"

using namespace std;

namespace gpp
{

gpp_client::gpp_client(gpp_networkinterface* hcon):
gpp_peer(hcon), EventQueue()
{
port=0;
ipaddress="";
}

gpp_client::~gpp_client()
{
}

uint16 gpp_client::getPort()const
{
return this->port;
}

string gpp_client::getIpAddress()const
{
return this->ipaddress;
}

/**
*Se conecta ao servidor e faz a altenticação.
*Caso a altenticação falhe, a conexão também falhará.
*Retorna falso se já estiver conectado...
**/
bool gpp_client::connect(const string& address, uint16 port)
{
_GASSERT_MSG(hcon!=NULL, "A steamnetworkingsockets não foi iniciada!");
if((address.size()==0)||(port==0)||(this->getHState()!=PEER_DEFAULT))
{
return false;
}
GPPCONNECTIONCALLBACK hcall=std::bind(&gpp_client::netCallBack, this, std::placeholders::_1, std::placeholders::_2);
uint32 sock=hcon->connectToServer(address, port, hcall);
if(sock>0)
{
this->port=port;
this->ipaddress=address;
this->setPeerId(sock);
hstate.store(PEER_ALT);
if(!altConnection())
{
this->shutdown();
return false;
}
return true;
}
return false;
}

bool gpp_client::altConnection()
{
if(!this->isAlt())
{
return false;
}
if((!this->isBot())&&(!this->isRegularClient()))
{
return false;
}
uint32 ptype=((isBot()) ? PEER_BOT : PEER_REGULAR_CLIENT);
string n=this->getPeerName();
string key="default password";
packet pack, result;
pack.createAlt(ptype, n, key);
setHState(PEER_CONNECTED);
return true;
}

void gpp_client::shutdown()
{
_GASSERT_MSG(hcon!=NULL, "A steamnetworkingsockets não foi iniciada!");
if(isConnected())
{
this->disconnect();
bool onforse=true;
int64 start=get_timestamp_ms();
while((get_timestamp_ms()-start)<2000)
{
wait_ms(20);
pollNet();
if(getHState()!=PEER_CONNECTED)
{
onforse=false;
break;
}
}
if(onforse)
{
this->disconnectNow();
this->setHState(PEER_DISCONNECTED);
}
}
}

/**
*Entra em loop enquanto a conexão estiver aberta.
*Todo o processo de loop é feito aqui, como ler o socket do servidor, processar eventos, etc...
*Isto é útil para deixar em um thread secundário...
**/
void gpp_client::run()
{
while(hstate.load()==PEER_CONNECTED)
{
wait_ms(5);
this->update();
}
}

void gpp_client::update()
{
pollNet();
pollEvents();
}

/**
*Faça tudo o que for relacionado a internet aqui...
**/
void gpp_client::pollNet()
{
_GASSERT_MSG(hcon!=NULL, "A steamnetworkingsockets não foi iniciada!");
switch(getHState())
{
case PEER_ALT:
case PEER_CONNECTED:
case PEER_DISCONNECTING:
{
hcon->onLoop();
vector<string> msgs;
if(hcon->receiveMessages(this->getPeerId(), msgs, 10)==0)
{
return;
}
for(auto& it : msgs)
{
processNetMessage(it);
}
break;
}
}
}

void gpp_client::pollEvents()
{
Event* ev=NULL;
while(eventGet(&ev))
{
dispatchEvent(ev);
delete ev;
}
}

 void gpp_client::processNetMessage(const string& msg)
{
packet* pack=new packet();
if(!pack->deserialize(msg))
{
delete pack;
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
Event* ev=new Event;
ev->type=GEVENT_PING;
ev->data=to_string(end-onping.load());
eventPost(ev);
onping.store(0);
}
delete pack;
break;
}
//Caso seja um outro tipo de pacote, gere o evento e despache ele para a fila de eventos.
default:
{
Event* ev=new Event();
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
void gpp_client::dispatchEvent(Event* ev)
{
if(ev==NULL)
{
return;
}
switch(ev->type)
{
//O servidor respondeu ao pacote ping, então aqui está o resultado...
case GEVENT_PING:
{
break;
}
}
}

//Private metods...

/**
*Callback para mudança de estados na conexão.
**/
void gpp_client::netCallBack(uint32 event, uint32 peer_id)
{
switch(event)
{
case GEVENT_CONNECTED:
{
this->setPeerId(peer_id);
this->setHState(PEER_CONNECTED);
break;
}
case GEVENT_DISCONNECTED:
{
this->disconnectNow();
this->setHState(PEER_DISCONNECTED);
break;
}
}
}
}

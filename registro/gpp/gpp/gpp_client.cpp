
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
#include<set>
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

gpp_client::gpp_client():
gpp_peer(), EventQueue()
{
port=0;
ipaddress="";
}

gpp_client::~gpp_client()
{
for(auto it=answers.begin(); it!=answers.end(); ++it)
{
delete it->second.second;
}
answers.clear();
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
this->setHState(PEER_CONNECTING);
EVENTPOSTCALLBACK  hcall=std::bind(&gpp_client::eventPostCallback, this, std::placeholders::_1);
uint32 sock=hcon->connectToServer(address, port, hcall);
if(sock>0)
{
this->port=port;
this->ipaddress=address;
this->setPeerId(sock);
hstate.store(PEER_CONNECTED);
return true;
}
this->setHState(PEER_DEFAULT);
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
this->disconnect();
}

/**
*Entra em loop enquanto a conexão estiver aberta.
*Todo o processo de loop é feito aqui, como ler o socket do servidor, processar eventos, etc...
*Isto é útil para deixar em um thread secundário...
**/
void gpp_client::run()
{
bool done=false;
while(done==false)
{
wait_ms(5);
this->update();
switch(getHState())
{
case PEER_DEFAULT:
case PEER_DISCONNECTED:
{
done=true;
break;
}
}
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
pollAnswers();
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

void gpp_client::pollAnswers()
{
if(answers.size()==0) return;
set<string> hdeletes;
int64 ms_time=get_timestamp_ms();
for(auto it=answers.begin(); it!=answers.end(); ++it)
{
if((ms_time-it->second.first)>=60000)
{
hdeletes.insert(it->first);
}
}
for(auto& it : hdeletes)
{
auto it2=answers.find(it);
_GASSERT(it2==answers.end());
delete it2->second.second;
answers.erase(it);
}
}

void gpp_client::pushAnswer(packet* hpack)
{
auto it=answers.find(hpack->command);
if(it!=answers.end())
{
delete it->second.second;
answers.erase(it);
}
answers.insert(make_pair(hpack->command, make_pair(get_timestamp_ms(), hpack)));
}

bool gpp_client::getAnswer(const std::string& cmd, packet** hpack)
{
auto it=answers.find(cmd);
if(it==answers.end()) return false;
if(hpack==NULL) return true;
*hpack=it->second.second;
answers.erase(it);
return true;
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
case PACKET_ANSWER:
{
pushAnswer(pack);
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
_GINFO("O ping é de {} ms", ev->data);
break;
}
case GEVENT_CONNECTED:
{
setHState(PEER_CONNECTED);
_GINFO("Conexão estabelecida");
break;
}
case GEVENT_DISCONNECTED:
{
setPeerId(0);
setHState(PEER_DISCONNECTED);
_GINFO("Conexão encerrada.");
break;
}
}
}

//Private metods...

void gpp_client::eventPostCallback(Event* ev)
{
EventQueue::eventPost(ev);
}
}

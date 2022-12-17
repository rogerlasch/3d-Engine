

/**
*Escrito por Roger em setembro de 2022.
*Este arquivo faz parte da game programming platform.
**/
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
#include"event.h"
#include"EventQueue.h"
#include"gpp_networkinterface.h"
#include"gpp_peer.h"
#include"gpp_server.h"
#include"ScopedFunction.h"

using namespace std;

namespace gpp
{

gpp_server::gpp_server(gpp_networkinterface* hcon):
EventQueue()
{
this->hcon=hcon;
hstate.store(SERVER_DEFAULT);
listensock=0;
}

gpp_server::~gpp_server()
{
if(hstate.load()==SERVER_RUNNING)
{
this->shutdown();
}
hcon=NULL;
}

uint16 gpp_server::getPort()const
{
return this->listenport;
}

uint32 gpp_server::playerCount()const
{
return peers.size();
}

uint32 gpp_server::getMaxPlayers()const
{
return this->max_players;
}

uint32 gpp_server::getHState()const
{
return hstate.load();
}

void gpp_server::setHState(uint32 hs)
{
this->hstate.store(hs);
}

gpp_networkinterface* gpp_server::getHCon()const
{
return this->hcon;
}

bool gpp_server::start(uint16 port, uint32 max_players)
{
_GASSERT_MSG(hcon!=NULL, "Nenhuma interface de rede definida!");
if((port==0)||(max_players==0)||(max_players>GPP_SERVER_DEFAULT_MAX_PLAYERS))
{
return false;
}
switch(hstate.load())
{
case SERVER_DEFAULT:
{
this->setHState(SERVER_STARTING);
GPPCONNECTIONCALLBACK hcall=std::bind(&gpp_server::netCallBack, this, std::placeholders::_1, std::placeholders::_2);
listensock=hcon->createListenSocket(port, hcall);
if(listensock>0)
{
this->listenport=port;
this->max_players=max_players;
this->hstate.store(SERVER_RUNNING);
return true;
}
break;
}
}
return false;
}

void gpp_server::shutdown()
{
switch(hstate.load())
{
case SERVER_STARTING:
case SERVER_RUNNING:
{
hstate.store(SERVER_SHUTING_DOWN);
peerDisconnectAll();
hcon->closeListenSocket(listensock);
listensock=0;
max_players=0;
hstate.store(SERVER_FINISHED);
break;
}
}
}

gpp_peer* gpp_server::getPeer(uint32 peer_id)const
{
auto it=peers.find(peer_id);
return ((it==peers.end()) ? NULL : it->second);
}

void gpp_server::peerDisconnectAll()
{
_GASSERT_MSG(hcon!=NULL, "Nenhuma interface de rede definida!");
if(playerCount()==0)
{
return;
}
for(auto it=peers.begin(); it!=peers.end(); ++it)
{
it->second->disconnect();
}
bool onforse=true;
int64 start=get_timestamp_ms();
while((get_timestamp_ms()-start)<2000)
{
wait_ms(25);
pollNet();
if(playerCount()==0)
{
onforse=false;
break;
}
}
}

void gpp_server::run()
{
_GASSERT_MSG(hstate.load()==SERVER_RUNNING, "O Servidor não está em um estado válido!");
while(hstate.load()==SERVER_RUNNING)
{
wait_ms(5);
this->update();
}
}

void gpp_server::update()
{
pollNet();
pollEvents();
}

void gpp_server::pollNet()
{
_GASSERT_MSG(hcon!=NULL, "Nenhuma interface de rede definida!");
hcon->onLoop();
for(auto it=peers.begin(); it!=peers.end(); ++it)
{
vector<string> msgs;
if(hcon->receiveMessages(it->first, msgs, 10)==0)
{
continue;
}
for(auto& it2 : msgs)
{
processNetMessage(it->second, it2);
}
}
}

void gpp_server::pollEvents()
{
Event* ev=NULL;
while(eventGet(&ev))
{
dispatchEvent(ev);
delete ev;
}
}

 void gpp_server::processNetMessage(gpp_peer* peer, const std::string& msg)
{
packet* pack=new packet();
if(!pack->deserialize(msg))
{
delete pack;
_FLOG("Pacote desconhecido: \"{}\"", msg);
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
Event* ev=new Event();
ev->type=GEVENT_RECEIVE;
ev->peer_id=peer->getPeerId();
ev->pack=pack;
eventPost(ev);
break;
}
//Caso seja um outro tipo de pacote, gere o evento e despache ele para a fila de eventos.
default:
{
Event* ev=new Event();
ev->type=GEVENT_RECEIVE;
ev->peer_id=peer->getPeerId();
ev->pack=pack;
ev->data=msg;
eventPost(ev);
break;
}
}
}

 void gpp_server::dispatchEvent(Event* ev)
{
_GASSERT_MSG(ev!=NULL, "O evento é nulo!!!");
switch(ev->type)
{
case GEVENT_PING:
{
_FLOG("O ping foi de {} ms.", ev->data);
break;
}
case GEVENT_RECEIVE:
{
gpp_peer* ch=peers.at(ev->peer_id);
ch->sendAnswerSuccess("Sua mensagem foi recebida com sucesso.");
break;
}
}
}

void gpp_server::netCallBack(uint32 event, uint32 peer_id)
{
switch(event)
{
case GEVENT_CONNECTED:
{
gpp_peer* p=new gpp_peer(PEER_CLIENT|PEER_SERVER, hcon);
p->setPeerId(peer_id);
p->setHState(PEER_ALT);
p->setConnectionTime(get_timestamp_ms());
peers.insert(make_pair(peer_id, p));
eventPost(peer_id, GEVENT_CONNECTED);
break;
}
case GEVENT_DISCONNECTED:
{
auto it=peers.find(peer_id);
if(it==peers.end())
{
return;
}
delete it->second;
peers.erase(it);
eventPost(peer_id, GEVENT_DISCONNECTED);
break;
}
}
}
}

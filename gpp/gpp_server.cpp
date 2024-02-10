

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
#include"debug_system.h"
#include"SafeFlags.h"
#include"packet.h"
#include"event.h"
#include"event.h"
#include"EventQueue.h"
#include"gpp_networkinterface.h"
#include"gpp_peer.h"
#include"gpp_server.h"

using namespace std;

namespace gpp
{

gpp_server::gpp_server():
EventQueue()
{
hState.setState(SERVER_DEFAULT);
listensock=0;
hcon=NULL;
}

gpp_server::~gpp_server()
{
if(hState.getState()==SERVER_RUNNING)
{
this->shutdown();
}
hcon=NULL;
}

bool gpp_server::start(uint16 port, uint32 max_players)
{
_GASSERT_MSG(hcon!=NULL, "Nenhuma interface de rede definida!");
if((port==0)||(max_players==0)||(max_players>GPP_SERVER_DEFAULT_MAX_PLAYERS))
{
return false;
}
switch(hState.getState())
{
case SERVER_DEFAULT:
{
this->setHState(SERVER_STARTING);
EVENTPOSTCALLBACK  hcall=std::bind(&gpp_server::eventPostCallback, this, std::placeholders::_1);
listensock=hcon->createListenSocket(port, hcall);
if(listensock>0)
{
this->listenport=port;
this->max_players=max_players;
this->hState.setState(SERVER_RUNNING);
return true;
}
this->hState.setState(SERVER_DEFAULT);
break;
}
}
return false;
}

void gpp_server::shutdown()
{
_GASSERT_MSG(hcon!=NULL, "Nenhuma interface de rede definida!");
switch(hState.getState())
{
case SERVER_STARTING:
case SERVER_RUNNING:
{
hState.setState(SERVER_SHUTING_DOWN);
peerDisconnectAll();
hcon->closeListenSocket(listensock);
listensock=0;
max_players=0;
hState.setState(SERVER_FINISHED);
break;
}
}
}

gpp_peer* gpp_server::getPeer(uint32 peer_id)const
{
auto it=peers.find(peer_id);
return ((it==peers.end()) ? NULL : it->second);
}

bool gpp_server::peerDisconnect(uint32 id){
gpp_peer* peer=getPeer(id);
if(peer==NULL)return false;
return peer->disconnect();
}

void gpp_server::peerDisconnectAll()
{
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
this_thread::yield();
pollNet();
pollEvents();
if(playerCount()==0)
{
onforse=false;
break;
}
}
}

void gpp_server::run()
{
_GASSERT_MSG(hState.getState()==SERVER_RUNNING, "O Servidor não está em um estado válido!");
while(hState.getState()==SERVER_RUNNING)
{
//wait_ms(5);
this_thread::yield();
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
int64 end_time=get_timestamp_ms();
if((it->second->isAlt())||((end_time-it->second->getConnectionTime())>10000))
{
it->second->disconnectNow();
continue;
}
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
//Caso seja um outro tipo de pacote, gere o evento e despache ele para a fila de eventos.
default:
{
Event* ev=new Event();
ev->type=GEVENT_RECEIVE;
ev->v1=peer->getPeerId();
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
case GEVENT_CONNECTED:
{
_GINFO("O par com id {} se conectou.", ev->v1);
gpp_peer* ch=new gpp_peer();
ch->setHcon(hcon);
ch->setPeerFlags(PEER_CLIENT|PEER_SERVER);
ch->setPeerId(ev->v1);
ch->setHState(PEER_CONNECTED);
ch->setConnectionTime(get_timestamp_ms());
peers.insert(make_pair(ev->v1, ch));
break;
}
case GEVENT_DISCONNECTED:
{
auto it=peers.find(ev->v1);
if(it!=peers.end())
{
_GINFO("O par com id {} se desconectou depois de {} ms...", ev->v1, get_timestamp_ms()-it->second->getConnectionTime());
delete it->second;
peers.erase(it);
}
else
{
_GINFO("Não achei.");
}
break;
}
case GEVENT_RECEIVE:
{
gpp_peer* ch=peers.at(ev->v1);
break;
}
}
}

void gpp_server::eventPostCallback(Event* ev)
{
EventQueue::eventPost(ev);
}
}

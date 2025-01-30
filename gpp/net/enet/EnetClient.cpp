
#include <enet/enet.h>
#include"../net.h"
#include"EnetClient.h"

using namespace std;
namespace gpp {

EnetClient::EnetClient()
: ClientController(){
hclient=NULL;
hpeer=NULL;
}

EnetClient::~EnetClient() {
}

bool EnetClient::connect(const string& address, uint16 port) {
    if (getState() != GNT_DEFAULT) {
trace("O cliente já está conectado ou foi finalizado.");
        return false;
    }

lock_guard<mutex> lck(mtx);

setState(GNT_SETUP);
this->address = address;
this->port = port;

        hclient = enet_host_create(nullptr, 1, 0, 0, 0);
if(!hclient){
trace("Erro ao criar host com enet. {}", __FUNCTION__);
return false;
}

        ENetAddress enetAddress;
        enet_address_set_host(&enetAddress, address.c_str());
        enetAddress.port = port;
hpeer = enet_host_connect(hclient, &enetAddress, 2, 0);
        if (!hpeer) {
            trace("Falha ao conectar ao servidor ENet {}", __FUNCTION__);
enet_host_destroy(hclient);
hclient=NULL;
            setState(GNT_DEFAULT);
            return false;
        }

return true;
}

void EnetClient::disconnect() {
uint32 st=getState();
if((st!=GNT_SETUP)&&(st!=GNT_RUNNING)){
trace("O cliente já está desconectado, ou está em processo de desconexão.");
return;
}

lock_guard<mutex> lck(mtx);

enet_peer_disconnect(hpeer, 0);
enet_host_flush(hclient);

setState(GNT_STOPPING);
trace("Desconectando cliente {}", __FUNCTION__);
}

void EnetClient::update() {

switch(getState()){
case GNT_SETUP:
case GNT_RUNNING:
case GNT_STOPPING:{
lock_guard<mutex> lck(mtx);
        ENetEvent event;
        enet_host_flush(hclient);
        while (enet_host_service(hclient, &event, 0) > 0) {
            switch (event.type) {
                case ENET_EVENT_TYPE_CONNECT:
setState(GNT_RUNNING);
                    trace("Conectado ao servidor");
                    sendEvent(GNET_CONNECTED, event.peer->incomingPeerID, "");
                    break;
                case ENET_EVENT_TYPE_RECEIVE:
                    sendEvent(GNET_DATA, event.peer->incomingPeerID, std::string((char*)event.packet->data, event.packet->dataLength));
                    enet_packet_destroy(event.packet);
                    break;
                case ENET_EVENT_TYPE_DISCONNECT:
                    trace("O cliente se desconectou do servidor.");
setState(GNT_STOPPED);
                    sendEvent(GNET_DISCONNECTED, event.peer->incomingPeerID, "");
                    break;
                default:
                    break;
            }
        }
break;
}
}
}

void EnetClient::shutdown() {
disconnect();
}

void EnetClient::send(const string& data, bool reliable, uint32 priority){
if(getState()==GNT_RUNNING){
lock_guard<mutex> lck(mtx);
        ENetPacket* packet = enet_packet_create(data.c_str(), data.length(), reliable ? ENET_PACKET_FLAG_RELIABLE : 0);
        enet_peer_send(hpeer, priority, packet);
        enet_host_flush(hclient);
}
}

int32 EnetClient::getPingTime()const{
lock_guard<mutex> lck(mtx);
if(hpeer!=NULL){
return hpeer->roundTripTime;
}
return -1;
}
}  // namespace gpp

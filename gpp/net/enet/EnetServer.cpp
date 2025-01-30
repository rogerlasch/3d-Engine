
#include <enet/enet.h>
#include"../net.h"
#include"EnetServer.h"

using namespace std;

namespace gpp {

EnetServer::EnetServer():
 ServerController(){
}

EnetServer::~EnetServer() {
}

bool EnetServer::listen(uint16_t port) {
    if (getState() != GNT_DEFAULT) {
trace("O servidor já está escutando ou foi finalizado.");
        return false;
    }

lock_guard<mutex> lck(mtx);
setState(GNT_SETUP);
    this->port = port;

        ENetAddress address;
        address.host = ENET_HOST_ANY;
        address.port = port;
        hserver = enet_host_create(&address, 1024, 0, 0, 0);

        if (!hserver) {
            trace("Falha ao criar servidor ENet {}", __FUNCTION__);
            setState(GNT_DEFAULT);
            return false;
        }

        trace("Servidor escutando na porta {}", port);
        setState(GNT_RUNNING);
    return true;
}

void EnetServer::sendTo(uint32 id, const string& data, bool reliable, uint32 priority) {
if(getState()==GNT_RUNNING){
lock_guard<mutex> lck(mtx);
    auto it = clients.find(id);
    if (it == clients.end()) {
trace("Client com id {} não localizado.", id);
        return;
    }

ENetPeer* hpeer=(ENetPeer*)it->second->getInternalData();
_GASSERT(hpeer!=NULL);

        ENetPacket* packet = enet_packet_create(data.c_str(), data.length(), reliable ? ENET_PACKET_FLAG_RELIABLE : 0);
        enet_peer_send(hpeer, priority, packet);
        enet_host_flush(hserver);
}
}

void EnetServer::broadcast(const string& data, bool reliable, uint32 priority) {
if(getState()==GNT_RUNNING){
lock_guard<mutex> lck(mtx);
        ENetPacket* packet = enet_packet_create(data.c_str(), data.length(), reliable ? ENET_PACKET_FLAG_RELIABLE : 0);
        enet_host_broadcast(hserver, priority, packet);
        enet_host_flush(hserver);
}
}

bool EnetServer::disconnect(uint32 id) {
lock_guard<mutex> lck(mtx);
    auto it = clients.find(id);
    if (it == clients.end()) {
trace("Cliente de id {} não localizado", id);
        return false;
    }

ENetPeer* hpeer=(ENetPeer*)it->second->getInternalData();
_GASSERT(hpeer!=NULL);
enet_peer_disconnect(hpeer, 0);
enet_host_flush(hserver);
trace("Desconexão de {} solicitada", id);
    return true;
}

void EnetServer::disconnect_all() {
trace("Desconexão de todos pares solicitada...");

for(auto& it : clients){
disconnect(it.first);
}
}

void EnetServer::update() {
cleanupPendings();
    switch(getState()) {
        case GNT_RUNNING:
        case GNT_STOPPING: {
            lock_guard<mutex> lck(mtx);
            ENetEvent event;

                        if ((getState() == GNT_STOPPING) && (clients.size() == 0)) {
if(hserver!=NULL){
                            enet_host_destroy(hserver);
                            hserver = NULL;
}
                            setState(GNT_STOPPED);
return;
                        }

            while (enet_host_service(hserver, &event, 0) > 0) {
                enet_host_flush(hserver);
                switch (event.type) {
                    case ENET_EVENT_TYPE_CONNECT: {
                        trace("Cliente conectado com ID {}", event.peer->incomingPeerID);
                        PeerInfo* hpeer = insertPeerInfo(event.peer->incomingPeerID, event.peer);
                        sendEvent(GNET_CONNECTED, event.peer->incomingPeerID, "");
                        break;
                    }
                    case ENET_EVENT_TYPE_RECEIVE: {
                        sendEvent(GNET_DATA, event.peer->incomingPeerID, string((char*)event.packet->data, event.packet->dataLength));
                        enet_packet_destroy(event.packet);
                        break;
                    }
                    case ENET_EVENT_TYPE_DISCONNECT: {
                        trace("Cliente desconectado com ID {}", event.peer->incomingPeerID);
                        removePeerInfo(event.peer->incomingPeerID);
                        sendEvent(GNET_DISCONNECTED, event.peer->incomingPeerID, "");

                        break;
                    }
                    default:
                        break;
                }
            }
            break;
        }
        default:
            break;
    }
}

void EnetServer::shutdown() {
trace("O servidor foi solicitado a encerrar.");
switch(getState()){
case GNT_SETUP:
case GNT_RUNNING:
    setState(GNT_STOPPING);
    disconnect_all();
break;
case GNT_STOPPING:
if(clientCount()==0){
lock_guard<mutex> lck(mtx);
enet_host_destroy(hserver);
hserver=NULL;
setState(GNT_STOPPED);
}
break;
}
}

int32 EnetServer::getPingTime(uint32 id)const{

lock_guard<mutex> lck(mtx);

auto it=clients.find(id);
if(it==clients.end()){
return -1;
}

ENetPeer* hpeer=(ENetPeer*)it->second->getInternalData();
_GASSERT(hpeer!=NULL);

return hpeer->roundTripTime;
}
}  // namespace gpp

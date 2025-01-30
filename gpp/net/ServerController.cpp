

#include <thread>
#include<string>
#include"../types.h"
#include"NetEvent.h"
#include"Packet.h"
#include"NetObject.h"
#include "ServerController.h"
#include"PeerInfo.h"

using namespace std;

namespace gpp {

ServerController::ServerController()
    : port(0), clientCounter(0) {
    setState(GNT_DEFAULT);
}

ServerController::~ServerController() {
    disconnect_all();  // Desconecta todos os clientes ao destruir o controlador
}

bool ServerController::listen(uint16 port) {
    if (getState() != GNT_DEFAULT) {
trace("O servidor já está escutando ou foi finalizado.");
        return false;
    }

setState(GNT_SETUP);
    this->port = port;
    setState(GNT_RUNNING);

setState(GNT_DEFAULT);
    return false;
}

void ServerController::sendTo(uint32 id, const std::string& data, bool reliable, uint32 priority) {
    auto it = clients.find(id);
    if (it == clients.end()) {
trace("Client com id {} não localizado.", id);
        return;
    }
}

void ServerController::sendTo(uint32 id, Packet* hpack, bool reliable, uint32 priority) {
sendTo(id, hpack->serialize(), reliable, priority);
packetPool.releaseObject(hpack);
}

void ServerController::broadcast(const std::string& data, bool reliable, uint32 priority) {
    // Envia dados para todos os clientes conectados
    for (const auto& client : clients) {
        sendTo(client.first, data, reliable, priority);
    }
}

void ServerController::broadcast(Packet* hpack, bool reliable, uint32 priority){
broadcast(hpack->serialize(), reliable, priority);
packetPool.releaseObject(hpack);
}

bool ServerController::disconnect(uint32 id) {
    auto it = clients.find(id);
    if (it == clients.end()) {
trace("Cliente de id {} não localizado", id);
        return false;
    }

    return true;
}

void ServerController::disconnect_all() {
    // Desconecta todos os clientes conectados
    for (auto& client : clients) {
        delete client.second;
    }
    clients.clear();
}

void ServerController::update() {
cleanupPendings();
switch(getState()){
case GNT_RUNNING:
case GNT_STOPPING:{
break;
}
}
}

void ServerController::shutdown() {
    disconnect_all();
    setState(GNT_STOPPED);
}

PeerInfo* ServerController::insertPeerInfo(uint32 id, void* data){
PeerInfo* hpeer=new PeerInfo();
hpeer->setId(id);
hpeer->setInternalData(data);
hpeer->adopt();
clients.insert(make_pair(id, hpeer));

return hpeer;
}

bool ServerController::removePeerInfo(uint32 id){

auto it=clients.find(id);
if(it!=clients.end()){

if(it->second->getRefCount()<=1){
it->second->release();
delete it->second;
clients.erase(it);
return true;
}

if(pendingDeletions.find(id)!=pendingDeletions.end()){
pendingDeletions.insert(id);
}

return false;
}
return false;
}

void ServerController::cleanupPendings(){
for(auto it=pendingDeletions.begin(); it!=pendingDeletions.end(); ++it){
if(removePeerInfo(*it)){
it=pendingDeletions.erase(it);
}
}
}


int32 ServerController::getPingTime(uint32 id)const{
return -1;
}
}  // namespace gpp

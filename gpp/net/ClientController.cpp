
#include <thread>
#include"../types.h"
#include"NetEvent.h"
#include"NetObject.h"
#include "ClientController.h"

using namespace std;

namespace gpp {

ClientController::ClientController() {
    // Estado inicial do objeto é GNT_DEFAULT
    setState(GNT_DEFAULT);
port=0;
address="";
}

ClientController::~ClientController() {
}

bool ClientController::connect(const string& address, uint16 port) {
    if (getState() != GNT_DEFAULT) {
trace("O cliente já está conectado ou foi finalizado.");
        return false;
    }

setState(GNT_SETUP);
this->address = address;
this->port = port;

trace("Tentando conectar em {} na porta {}", address, port);
setState(GNT_DEFAULT);
trace("Conexão falhou!");
return false;
}

void ClientController::disconnect() {
uint32 st=getState();
if((st!=GNT_SETUP)&&(st!=GNT_RUNNING)){
trace("O cliente já está desconectado, ou está em processo de desconexão.");
return;
}

setState(GNT_STOPPING);
setState(GNT_STOPPED);
trace("Cliente desconectado");
}

void ClientController::update() {
uint32 st=getState();
if((st==GNT_DEFAULT)||(st==GNT_STOPPED)){
return;
}
}

void ClientController::shutdown() {
disconnect();
}

void ClientController::send(const string& data, bool reliable, uint32 priority){
}

void ClientController::sendPacket(Packet* hpack, bool reliable, uint32 priority){
send(hpack->serialize(), reliable, priority);
packetPool.releaseObject(hpack);
}

void ClientController::waitForConnection(){
while(getState()==GNT_SETUP){
this_thread::sleep_for(chrono::milliseconds(20));
}
}
}  // namespace gpp

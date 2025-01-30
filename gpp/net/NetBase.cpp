
#include<chrono>
#include"NetCodes.h"
#include"../types.h"
#include"NetEvent.h"
#include "NetBase.h"

using namespace std;

namespace gpp {

NetBase::NetBase() {
    hstate.store(GNT_DEFAULT);  // Estado inicial é GNT_DEFAULT
}

NetBase::~NetBase() {
    stop_all();  // Garante que todos os objetos de rede serão encerrados ao destruir
}

void NetBase::start() {
    if (hstate.load() != GNT_DEFAULT) {
logger::debug("Estado errado para a inicialização de {}", __FUNCTION__);
        return;  // Somente inicia se o estado for GNT_DEFAULT
    }

    hstate.store(GNT_SETUP);

    runner = std::jthread([this](std::stop_token st) {
        hstate.store(GNT_RUNNING);
bool done=false;
        while (done==false) {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
if(st.stop_requested()){
hstate.store(GNT_STOPPING);
if(hnets.empty()){
done=true;
}
}
            update();
        }
        hstate.store(GNT_STOPPED);
    });
}

void NetBase::stop() {
    if (hstate.load() == GNT_RUNNING) {
        hstate.store(GNT_STOPPING);
stop_all();
        runner.request_stop();  // Solicita o encerramento da thread
    }
}

void NetBase::waitStop() {
    while (hstate.load() != GNT_STOPPED) {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));  // Espera até que o estado seja GNT_STOPPED
    }
}

void NetBase::stop_all() {
    std::lock_guard<std::mutex> lock(mtx);
    for (auto& it : hnets){
it->shutdown();
    }
}

void NetBase::update() {
    std::lock_guard<std::mutex> lock(mtx);

vector<NetObject*> objs;

    for (auto it = hnets.begin(); it != hnets.end(); ++it) {
        NetObject* netObj = *it;
        netObj->update();

        if (netObj->getState() == GNT_STOPPED) {
            netObj->end();
objs.push_back((*it));
//logger::info("Algo foi deletado");
        }
    }

if(objs.size()>0){
while(objs.size()>0){
auto it=std::find(hnets.begin(), hnets.end(), objs[0]);
if(it!=hnets.end()){
hnets.erase(it);
delete objs[0];
}
objs.erase(objs.begin());
}
}
}

ClientController* NetBase::createNewClient(NETEVENTCALLBACK hcall) {
    if (hstate.load() == GNT_STOPPING || hstate.load() == GNT_STOPPED) {
        return nullptr;  // Não cria novos clientes enquanto a rede está fechando
    }
    // Lógica para criar um novo cliente
    return nullptr;  // Substitua isso pelo retorno correto de um objeto de cliente
}

ServerController* NetBase::createNewServer(NETEVENTCALLBACK hcall) {
    if (hstate.load() == GNT_STOPPING || hstate.load() == GNT_STOPPED) {
        return nullptr;  // Não cria novos servidores enquanto a rede está fechando
    }
    // Lógica para criar um novo servidor
    return nullptr;  // Substitua isso pelo retorno correto de um objeto de servidor
}
}  // namespace gpp


#include <enet/enet.h>
#include"../net.h"
#include"EnetBase.h"
#include"EnetClient.h"
#include"EnetServer.h"

using namespace std;

namespace gpp {

EnetBase::EnetBase()
: NetBase() {
}

EnetBase::~EnetBase() {
}

void EnetBase::loadDriver(){
int32 res=enet_initialize();
if(res!=0){
logger::debug("Erro ao inicializar a enet. {}", res);
}
}

void EnetBase::unloadDriver(){
enet_deinitialize();
}

ClientController* EnetBase::createNewClient(NETEVENTCALLBACK hcall) {
    if (hstate.load() == GNT_STOPPING || hstate.load() == GNT_STOPPED) {
        return nullptr;  // Não cria novos clientes enquanto a rede está fechando
    }

lock_guard<mutex> lck(mtx);
EnetClient* hc=new EnetClient();
hnets.push_back(hc);
hc->setEventCallback(hcall);
hc->init();

return hc;
}

ServerController* EnetBase::createNewServer(NETEVENTCALLBACK hcall) {
    if (hstate.load() == GNT_STOPPING || hstate.load() == GNT_STOPPED) {
        return nullptr;  // Não cria novos servidores enquanto a rede está fechando
    }

lock_guard<mutex> lck(mtx);
EnetServer* hs=new EnetServer();
hs->setEventCallback(hcall);
hs->init();
hnets.push_back(hs);
return hs;
}
}

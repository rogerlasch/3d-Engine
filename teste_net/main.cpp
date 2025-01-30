
#define GPP_DOUBLE_PRECISION
#include <iostream>
#include <gpp/gpp.h>
#include"g_start.h"

using namespace gpp;
using namespace std;

int main() {
G_STARTER gst;
NETEVENTCALLBACK hservercall=[](NetEvent* ev){
switch(ev->type){
case GNET_CONNECTED:
logger::info("{} se conectou", ev->id);
break;
case GNET_DISCONNECTED:
logger::info("{} se desconectou", ev->id);
break;
case GNET_DATA:
logger::info("{} enviou {}", ev->id, ev->data);
break;
case GNET_LOG:
cout<<ev->data<<endl;
logger::info(ev->data);
break;
}
};
NETEVENTCALLBACK hclientcall=[](NetEvent* ev){
switch(ev->type){
case GNET_CONNECTED:
logger::info("Eu, cliente me conectei no servidor.");
break;
case GNET_DISCONNECTED:
logger::info("Eu, cliente me desconectei do servidor ou fui solicitado a isso.");
break;
case GNET_DATA:
logger::info("Recebi \"{}\" do servidor.", ev->data);
break;
case GNET_LOG:
logger::info("eu cliente \"{}\"", ev->data);
break;
}
};
NetBase* hbase=createEnetBase();
hbase->loadDriver();
hbase->start();
ServerController* hserver=hbase->createNewServer(hservercall);
hserver->listen(2500);
/*
ClientController* hclient=hbase->createNewClient(hclientcall);
hclient->connect("127.0.0.1", 2500);
hclient->waitForConnection();
hclient->send("E aí mano, olha eu aqui porra.", true, 0);
hclient->send("To indo embora agora velho.");
*/
wait_ms(5000);
/*
logger::info("Ping do cliente para o servidor: {}", hclient->getPingTime());
*/
hbase->stop();
logger::info("Esperando a base finalizar.");
hbase->waitStop();
logger::info("Base finalizada!");
return 0;
}

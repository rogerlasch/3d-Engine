
#include<signal.h>
#include"../gpp/gpp.h"

using namespace std;
using namespace gpp;

unique_ptr<gpp_steamsockets> hnet;
unique_ptr<gpp_client> hclient;
#define FILENAME "cliente.txt"

void sig_callback(int x);
int main()
{
setlocale(LC_ALL, "Portuguese");
signal(SIGTERM, sig_callback);
signal(SIGINT, sig_callback);
signal(SIGABRT, sig_callback);
log_create(FILENAME, LG_ALL);
log_set_default(FILENAME);
hnet=make_unique<gpp_steamsockets>();
hclient=make_unique<gpp_client>();
hclient->setHcon(hnet.get());
_GINFO("se conectando em localhost... {}", hclient->connect("127.0.0.1", 4000));
while(hclient->isConnected()){
this_thread::yield();
hclient->makePing();
hclient->update();
}
hclient.reset();
hnet.reset();
return 0;
}

void sig_callback(int x)
{
hclient->shutdown();
}

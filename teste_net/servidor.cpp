
#include<signal.h>
#include"../gpp/gpp.h"

using namespace std;
using namespace gpp;

unique_ptr<gpp_steamsockets> hnet;
unique_ptr<gpp_server> hserver;
#define FILENAME "servidor.txt"

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
hserver=make_unique<gpp_server>();
hserver->setHcon(hnet.get());
hserver->start(4000, 50);
hserver->run();
hserver.reset();
hnet.reset();
_GINFO("Servidor finalizado com sucesso");
return 0;
}

void sig_callback(int x)
{
_GINFO("Parando servidor...");
hserver->shutdown();
}

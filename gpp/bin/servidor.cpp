

#include<signal.h>
#include<iostream>
#include"../gpp.h"

using namespace std;
using namespace gpp;

basic_server server;
atomic<uint32> hstate;
void sig_callback(int x);
int main()
{
cout<<"hmm"<<endl;
log_open_file("servidor_log.txt");
profiler_start();
setlocale(LC_ALL, "Portuguese");
signal(SIGTERM, sig_callback);
signal(SIGINT, sig_callback);
signal(SIGABRT, sig_callback);
_FLOG("Iniciando servidor...");
if(server.start(4000, 50))
{
_FLOG("Servidor iniciado.");
}
else
{
_FLOG("Erro ao iniciar o servidor...");
return 0;
}
server.run();
profiler_dump("profiler_servidor.txt");
return 0;
}

void sig_callback(int x)
{
server.shutdown();
}

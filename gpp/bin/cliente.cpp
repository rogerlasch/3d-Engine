

#include<signal.h>
#include<iostream>
#include"..\gpp.h"

using namespace std;
using namespace gpp;

gpp_steamsockets hnet;
gpp_client* client=NULL;
atomic<uint32> hstate;
void sig_callback(int x);
void client_loop();
int main()
{
log_open_file("client_log.txt");
profiler_start();
setlocale(LC_ALL, "Portuguese");
signal(SIGTERM, sig_callback);
signal(SIGINT, sig_callback);
signal(SIGABRT, sig_callback);
hstate.store(1);
thread th2(client_loop);
while(hstate.load()==1)
{
this_thread::sleep_for(chrono::milliseconds(25));
string line="";
getline(cin, line);
if(line.size()>0)
{
if(line=="ping")
{
client->makePing();
}
else
{
packet sf;
if(!client->sendAndWait(4, "Estou testando o envio...", &sf))
{
cout<<"Mensagem de falha: "<<sf.errormsg<<endl;
}
else
{
_FLOG("Mensagem recebida: {}", sf.errormsg);
}
}
}
}
th2.join();
_FLOG("Programa finalizado.");
profiler_dump("client_profiler.txt");
return 0;
}

void sig_callback(int x)
{
client->shutdown();
wait_ms(100);
hstate.store(0);
}

void client_loop()
{
profiler_snap();
_FLOG("Iniciando cliente...");
client=new gpp_client(PEER_REGULAR_CLIENT|PEER_CLIENT, &hnet);
if(client->connect("127.0.0.1", 4000))
{
_FLOG("Cliente iniciado.");
}
else
{
hstate.store(0);
_FLOG("Erro ao iniciar o cliente...");
delete client;
return;
}
while(client->isConnected())
{
wait_ms(2);
client->update();
}
hstate.store(0);
delete client;
}



#include<signal.h>
#include<iostream>
#include"gpp.h"

using namespace std;
using namespace gpp;

basic_server server;
basic_client client;
atomic<uint32> hstate;
void sig_callback(int x);
void server_loop();
void client_loop();
int main()
{
setlocale(LC_ALL, "Portuguese");
signal(SIGTERM, sig_callback);
signal(SIGINT, sig_callback);
signal(SIGABRT, sig_callback);
hstate.store(1);
thread th1(server_loop);
thread th2(client_loop);
while(hstate.load()==1)
{
this_thread::sleep_for(chrono::milliseconds(5));
string line="";
getline(cin, line);
if(line.size()>0)
{
client.sendReliable(line);
}
}
client.disconnect();
server.shutdown();
th1.join();
th2.join();
_FLOG("Programa finalizado.");
return 0;
}

void sig_callback(int x)
{
hstate.store(0);
}

void server_loop()
{
_FLOG("Iniciando servidor...");
if(server.start(4000, 50))
{
_FLOG("Servidor iniciado.");
}
else
{
hstate.store(0);
_FLOG("Erro ao iniciar o servidor...");
return;
}
while(hstate.load()==1)
{
this_thread::sleep_for(chrono::milliseconds(5));
server.update();
}
}

void client_loop()
{
this_thread::sleep_for(chrono::milliseconds(3000));
_FLOG("Iniciando cliente...");
if(client.connect("127.0.0.1", 4000))
{
_FLOG("Cliente iniciado.");
}
else
{
hstate.store(0);
_FLOG("Erro ao iniciar o cliente...");
return;
}
while(hstate.load()==1)
{
this_thread::sleep_for(chrono::milliseconds(5));
client.update();
if(client.isDisconnected())
{
hstate.store(0);
}
}
}

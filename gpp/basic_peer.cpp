
/**
*Escrito por Roger em setembro de 2022.
*Este arquivo faz parte da game programming platform.
*Classe de comunica��o b�sica.
*Cont�m m�todos para envio de mensagens entre o servidor e cliente.
**/
#include <GameNetworkingSockets/steam/steamnetworkingsockets.h>
#include <GameNetworkingSockets/steam/isteamnetworkingutils.h>
#include<atomic>
#include<functional>
#include<mutex>
#include<memory>
#include<shared_mutex>
#include<string>
#include"types.h"
#include"debug.h"
#include"profiler.h"
#include"ScopedFunction.h"
#include"time.h"
#include"SafeFlags.h"
#include"packet.h"
#include"PacketWaitable.h"
#include"event.h"
#include"EventQueue.h"
#include"GppSteamBase.h"
#include"GppClientServerBase.h"
#include"basic_peer.h"

using namespace std;

namespace gpp
{
basic_peer::basic_peer(uint32 ctype, uint32 hclass)
{
peer_id=0;
hserver=NULL;
connection_time=0;
hstate=PEER_DEFAULT;
this->hclass=hclass;
this->ctype.store(ctype);
onping.store(0);
hpack=make_shared<PacketWaitable>();
}

basic_peer::~basic_peer()
{
hpack.reset();
}

bool basic_peer::isBot()const
{
return ctype.load()==PEER_BOT;
}

bool basic_peer::isRegularClient()const
{
return ctype.load()==PEER_REGULAR_CLIENT;
}

bool basic_peer::isOnClient()const
{
return this->hclass==GNET_CLIENT;
}

bool basic_peer::isOnServer()const
{
return this->hclass==GNET_SERVER;
}

bool basic_peer::isConnected()const
{
return hstate.load()==PEER_CONNECTED;
}

bool basic_peer::isDisconnected()const
{
return hstate.load()==PEER_DISCONNECTED;
}

bool basic_peer::isAlt()const
{
return hstate.load()==PEER_ALT;
}

//Esta propriedade n�o deve ser alterada depois que a classe for instanciada e configurada.
uint32 basic_peer::getHclass()const
{
return this->hclass;
}

//Esta propriedade n�o deve ser alterada depois que a classe for instanciada e configurada.
uint32 basic_peer::getCtype()const
{
return this->ctype.load();
}

void basic_peer::setCtype(uint32 ctype)
{
this->ctype.store(ctype);
}

uint32  basic_peer::getPeerId()const
{
shared_lock<shared_mutex> lck(mtx_con);
return peer_id;
}

void  basic_peer::setPeerId(uint32 peer_id)
{
unique_lock<shared_mutex> lck(mtx_con);
this->peer_id=peer_id;
}

uint32  basic_peer::getHstate()const
{
return hstate.load();
}

void  basic_peer::setHstate(uint32 hstate)
{
this->hstate.store(hstate);
}

int64  basic_peer::getConnectionTime()const
{
shared_lock<shared_mutex> lck(mtx_con);
return this->connection_time;
}

void  basic_peer::setConnectionTime(int64 connection_time)
{
unique_lock<shared_mutex> lck(mtx_con);
this->connection_time=connection_time;
}

ISteamNetworkingSockets*  basic_peer::getHserver()const
{
shared_lock<shared_mutex> lck(mtx_con);
return this->hserver;
}

void  basic_peer::setHserver(ISteamNetworkingSockets* hserver)
{
unique_lock<shared_mutex> lck(mtx_con);
this->hserver=hserver;
}

/**
*Envia uma mensagem sem garantia de recebimento pela internet.
*Este m�todo est� dispon�vel tanto pelo servidor como pelo cliente.
*Retorna true caso o envio seja bem sucedido.
**/
bool basic_peer::sendUnreliable(const std::string& data)
{
profiler_snap();
unique_lock<shared_mutex> lck(mtx_con);
if((hserver==NULL)||(peer_id==0))
{
return false;
}
uint32 x=hstate.load();
uint32 res=0;
switch(x)
{
case PEER_ALT:
case PEER_CONNECTED:
case PEER_DISCONNECTING:
{
res=hserver->SendMessageToConnection(peer_id, data.c_str(), data.size(), k_nSteamNetworkingSend_Unreliable, NULL);
break;
}
}
return res==k_EResultOK;
}

/**
*Envia uma mensagem confi�vel pela internet.
*O par envia uma e outra vez a mensagem at� receber uma confirma��o de recebimento, ou at� que algo diferente aconte�a.
*retorna true se obtiver sucesso.
**/
bool basic_peer::sendReliable(const std::string& data)
{
profiler_snap();
unique_lock<shared_mutex> lck(mtx_con);
if((hserver==NULL)||(peer_id==0))
{
return false;
}
uint32 x=hstate.load();
uint32 res=0;
switch(x)
{
case PEER_ALT:
case PEER_CONNECTED:
case PEER_DISCONNECTING:
{
res=hserver->SendMessageToConnection(peer_id, data.c_str(), data.size(), k_nSteamNetworkingSend_Reliable, NULL);
break;
}
}
return res==k_EResultOK;
}

/**
*Cria um pacote e tenta enviar de forma n�o confi�vel pela internet.
**/
bool basic_peer::sendPacketUnreliable(uint32 type, uint32 flags, int64 timeout, const std::string& data)
{
packet pack;
pack.type=type;
pack.flags=flags;
pack.timeout=timeout;
pack.setCommand(data, type);
if(!pack.isValid())
{
return false;
}
return sendUnreliable(pack.serialize());
}

/**
*Cria e transmite um pacote confi�vel pela internet.
**/
bool basic_peer::sendPacketReliable(uint32 type, uint32 flags, int64 timeout, const std::string& data)
{
packet pack;
pack.type=type;
pack.flags=flags;
pack.timeout=timeout;
pack.setCommand(data, type);
if(!pack.isValid())
{
return false;
}
return sendReliable(pack.serialize());
}

/**
*Envia um pacote do tipo PACKET_WAITABLE para o servidor.
*Este m�todo deve ser usado s� pelo cliente.
*Note que este m�todo esperar� at� que o servidor responda, ou o tempo limite seja atingido.
*Um exemplo de uso � quando o cliente solicita dados espec�ficos para o servidor e precisa esperar at� ter os dados em m�os para continuar.
*O time out atualmente � de 15 segundos.
*S� mente um pacote do tipo waitable pode ser enviado por vez.
*Verifique o membro errorcode  do par�metro result para analisar o que de fato aconteceu.
*Retorna true se o servidor respondeu, ou false se o timeout foi atingido.
**/
bool basic_peer::sendAndWait(uint32 type, const string& data, packet* result)
{
profiler_snap();
if(result==NULL)
{
return false;
}
switch(getHclass())
{
case GNET_CLIENT:
{
if(hpack->isWaiting())
{
result->errorcode=PWAITABLE_ERROR;
result->errormsg="A solicita��o n�o pode ser processada agora.";
return false;
}
shared_ptr<unique_lock<mutex>> lck=make_shared<unique_lock<mutex>>(hpack->mtx);
hpack->setflag(PWF_WAITING);
packet sf;
sf.type=PACKET_WAITABLE;
sf.subtype=type;
sf.data=data;
hpack->psend=&sf;
hpack->presult=NULL;
hpack->timestamp.store(get_timestamp_ms());
hpack->timeout.store(15000);
string str=sf.serialize();
if(!sendReliable(str))
{
lck.reset();
hpack->cleanup();
result->errorcode=PWAITABLE_ERROR;
result->errormsg="Voc� n�o est� conectado, ou a internet falhou!";
return false;
}
lck->unlock();
//Espera pela resposta, ou at� o timeout acontecer...
while(this->isConnected())
{
wait_ms(20);
//this->updateCon();
int64 end=get_timestamp_ms();
if((hpack->flag_contains(PWF_READ))||((end-hpack->timestamp.load())>hpack->timeout.load()))
{
break;
}
}
lck->lock();
//O timeout aconteceu?
if(!hpack->flag_contains(PWF_READ))
{
result->errorcode=PWAITABLE_ERROR;
result->errormsg="O pedido n�o foi respondido dentro do tempo especificado.";
lck.reset();
hpack->cleanup();
return false;
}
//Copia o pacote recebido para o par�metro result...
*result=*hpack->presult;
delete hpack->presult;
lck.reset();
hpack->cleanup();
//Retorna true mesmo que o servidor tenha respondido de forma negativa.
//Porque o objetivo � enviar uma pergunta e o servidor responder.
return true;
}
}
return false;
}

/**
*Responde de forma positiva para o cliente.
*Este m�todo � usado em resposta a um pacote waitable.
*Isto pode ser usado em diversas situa��es desde uma mensagem simples para o usu�rio, como algo mais complexo como envio de um mapa inteiro para o cliente.
*Este m�todo deve ser usado s� pelo servidor.
**/
bool basic_peer::sendAnswerSuccess(const string& data)
{
profiler_snap();
switch(getHclass())
{
case GNET_SERVER:
{
if(hpack->psend==NULL)
{
_FLOG("Erro, o servidor n�o sabe o que responder exatamente... Dados a enviar: \"{}\"", data);
return false;
}
packet pack;
pack.type=PACKET_ANSWER_WAITABLE;
pack.subtype=hpack->psend->subtype;
pack.errorcode=PWAITABLE_OK;
pack.errormsg="A opera��o foi conclu�da com sucesso.";
pack.command=hpack->psend->command;
pack.data=data;
string str=pack.serialize();
return sendReliable(str);
}
}
return false;
}

/**
*Responde de forma negativa para o cliente.
*Mesma coisa que o m�todo sendAnswerSuccess, por�m usado para informar uma falha, ou erro.
*Deve ser usado s� pelo servidor.
**/
bool basic_peer::sendAnswerError(const string& errormsg)
{
profiler_snap();
switch(getHclass())
{
case GNET_SERVER:
{
if(hpack->psend==NULL)
{
_FLOG("Erro, o servidor n�o sabe o que responder exatamente... Dados a enviar: \"{}\"",
errormsg);
return false;
}
packet pack;
pack.type=PACKET_ANSWER_WAITABLE;
pack.subtype=hpack->psend->subtype;
pack.errorcode=PWAITABLE_ERROR;
pack.errormsg=errormsg;
pack.command=hpack->psend->command;
pack.data="";
string str=pack.serialize();
return sendReliable(str);
}
}
return false;
}

/**
*Fecha a conex�o enviando os pacotes restantes...
*Neste estado, qualquer chamada para os m�todos sendReliable e sendUnreliable devem falhar.
**/
bool basic_peer::disconnect()
{
profiler_snap();
unique_lock<shared_mutex> lck(mtx_con);
if((hserver==NULL)||(peer_id==0))
{
return false;
}
if(hserver->CloseConnection(peer_id, 0, NULL, true))
{
hstate.store(PEER_DISCONNECTING);
return true;
}
return false;
}

/**
*Fecha a conex�o imediatamente.
*Quaisquer pacotes pendentes ser�o perdidos.
**/
bool basic_peer::disconnectNow()
{
profiler_snap();
unique_lock<shared_mutex> lck(mtx_con);
if((hserver==NULL)||(peer_id==0))
{
return false;
}
if(hserver->CloseConnection(peer_id, 0, NULL, false))
{
hstate.store(PEER_DISCONNECTED);
return true;
}
return false;
}

/**
*Fa�a uma solicita��o ping.
*Isto normalmente � usado pelo cliente, mas tamb�m pode ser usado pelo servidor, apesar de n�o fazer muito sentido.
*O ping � dado como morto a cima de 10 segundos. Ou seja, enquanto o ping estiver ativo, outra solicita��o n�o pode ser feita.
**/
void basic_peer::makePing()
{
profiler_snap();
if(onping.load()>0)
{
return;
}
onping.store(get_timestamp_ms());
sendPacketReliable(PACKET_PING, 0, 0, "");
}
}


/**
*Escrito por Roger em setembro de 2022.
*Este arquivo faz parte da game programming platform.
*Classe de comunicação básica.
*Contém métodos para envio de mensagens entre o servidor e cliente.
**/
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
#include"gpp_networkinterface.h"
#include"gpp_peer.h"

using namespace std;

namespace gpp
{
gpp_peer::gpp_peer(uint32 pflags, gpp_networkinterface* hcon)
{
peer_id=0;
connection_time=0;
hstate=PEER_DEFAULT;
this->pflags.replace_flags(pflags);
this->hcon=hcon;
onping.store(0);
hpack=make_shared<PacketWaitable>();
}

gpp_peer::~gpp_peer()
{
hpack.reset();
hcon=NULL;
}

bool gpp_peer::isBot()const
{
return pflags.flag_contains(PEER_BOT);
}

bool gpp_peer::isRegularClient()const
{
return pflags.flag_contains(PEER_REGULAR_CLIENT);
}

bool gpp_peer::isOnClient()const
{
return pflags.flag_contains(PEER_CLIENT);
}

bool gpp_peer::isOnServer()const
{
return pflags.flag_contains(PEER_SERVER);
}

bool gpp_peer::isConnected()const
{
return hstate.load()==PEER_CONNECTED;
}

bool gpp_peer::isDisconnected()const
{
return hstate.load()==PEER_DISCONNECTED;
}

bool gpp_peer::isAlt()const
{
return hstate.load()==PEER_ALT;
}

uint32 gpp_peer::getPFlags()const
{
return pflags.get_flags();
}

void gpp_peer::setPFlags(uint32 pflags)
{
this->pflags.replace_flags(pflags);
}

//Esta propriedade não deve ser alterada depois que a classe for instanciada e configurada.
uint32  gpp_peer::getPeerId()const
{
shared_lock<shared_mutex> lck(mtx_con);
return peer_id;
}

void  gpp_peer::setPeerId(uint32 peer_id)
{
unique_lock<shared_mutex> lck(mtx_con);
this->peer_id=peer_id;
}

uint32  gpp_peer::getHState()const
{
return hstate.load();
}

void  gpp_peer::setHState(uint32 hstate)
{
this->hstate.store(hstate);
}

int64  gpp_peer::getConnectionTime()const
{
shared_lock<shared_mutex> lck(mtx_con);
return this->connection_time;
}

void  gpp_peer::setConnectionTime(int64 connection_time)
{
unique_lock<shared_mutex> lck(mtx_con);
this->connection_time=connection_time;
}

gpp_networkinterface*  gpp_peer::getHcon()const
{
shared_lock<shared_mutex> lck(mtx_con);
return this->hcon;
}

void  gpp_peer::setHcon(gpp_networkinterface* hcon)
{
unique_lock<shared_mutex> lck(mtx_con);
this->hcon=hcon;
}

/**
*Envia uma mensagem sem garantia de recebimento pela internet.
*Este método está disponível tanto pelo servidor como pelo cliente.
*Retorna true caso o envio seja bem sucedido.
**/
bool gpp_peer::sendUnreliable(const std::string& data)
{
profiler_snap();
unique_lock<shared_mutex> lck(mtx_con);
if((hcon==NULL)||(peer_id==0))
{
return false;
}
uint32 x=hstate.load();
switch(x)
{
case PEER_ALT:
case PEER_CONNECTED:
case PEER_DISCONNECTING:
{
return hcon->sendUnreliable(peer_id, data)>0;
break;
}
}
return false;
}

/**
*Envia uma mensagem confiável pela internet.
*O par envia uma e outra vez a mensagem até receber uma confirmação de recebimento, ou até que algo diferente aconteça.
*retorna true se obtiver sucesso.
**/
bool gpp_peer::sendReliable(const std::string& data)
{
profiler_snap();
unique_lock<shared_mutex> lck(mtx_con);
if((hcon==NULL)||(peer_id==0))
{
return false;
}
uint32 x=hstate.load();
switch(x)
{
case PEER_ALT:
case PEER_CONNECTED:
case PEER_DISCONNECTING:
{
return hcon->sendReliable(peer_id, data)>0;
}
}
return false;
}

/**
*Cria um pacote e tenta enviar de forma não confiável pela internet.
**/
bool gpp_peer::sendPacketUnreliable(uint32 type, uint32 flags, int64 timeout, const std::string& data)
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
*Cria e transmite um pacote confiável pela internet.
**/
bool gpp_peer::sendPacketReliable(uint32 type, uint32 flags, int64 timeout, const std::string& data)
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
*Este método deve ser usado só pelo cliente.
*Note que este método esperará até que o servidor responda, ou o tempo limite seja atingido.
*Um exemplo de uso é quando o cliente solicita dados específicos para o servidor e precisa esperar até ter os dados em mãos para continuar.
*O time out atualmente é de 15 segundos.
*Só mente um pacote do tipo waitable pode ser enviado por vez.
*Verifique o membro errorcode  do parâmetro result para analisar o que de fato aconteceu.
*Retorna true se o servidor respondeu, ou false se o timeout foi atingido.
**/
bool gpp_peer::sendAndWait(uint32 type, const string& data, packet* result)
{
profiler_snap();
if(result==NULL)
{
return false;
}
if(isOnClient())
{
if(hpack->isWaiting())
{
result->errorcode=PWAITABLE_ERROR;
result->errormsg="A solicitação não pode ser processada agora.";
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
result->errormsg="Você não está conectado, ou a internet falhou!";
return false;
}
lck->unlock();
//Espera pela resposta, ou até o timeout acontecer...
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
result->errormsg="O pedido não foi respondido dentro do tempo especificado.";
lck.reset();
hpack->cleanup();
return false;
}
//Copia o pacote recebido para o parâmetro result...
*result=*hpack->presult;
delete hpack->presult;
lck.reset();
hpack->cleanup();
//Retorna true mesmo que o servidor tenha respondido de forma negativa.
//Porque o objetivo é enviar uma pergunta e o servidor responder.
return true;
}
return false;
}

/**
*Responde de forma positiva para o cliente.
*Este método é usado em resposta a um pacote waitable.
*Isto pode ser usado em diversas situações desde uma mensagem simples para o usuário, como algo mais complexo como envio de um mapa inteiro para o cliente.
*Este método deve ser usado só pelo servidor.
**/
bool gpp_peer::sendAnswerSuccess(const string& data)
{
profiler_snap();
if(hpack->psend==NULL)
{
_FLOG("Erro, o servidor não sabe o que responder exatamente... Dados a enviar: \"{}\"", data);
return false;
}
if(isOnServer())
{
packet pack;
pack.type=PACKET_ANSWER_WAITABLE;
pack.subtype=hpack->psend->subtype;
pack.errorcode=PWAITABLE_OK;
pack.errormsg="A operação foi concluída com sucesso.";
pack.command=hpack->psend->command;
pack.data=data;
string str=pack.serialize();
return sendReliable(str);
}
return false;
}

/**
*Responde de forma negativa para o cliente.
*Mesma coisa que o método sendAnswerSuccess, porém usado para informar uma falha, ou erro.
*Deve ser usado só pelo servidor.
**/
bool gpp_peer::sendAnswerError(const string& errormsg)
{
profiler_snap();
if(hpack->psend==NULL)
{
_FLOG("Erro, o servidor não sabe o que responder exatamente... Dados a enviar: \"{}\"",
errormsg);
return false;
}
if(isOnServer())
{
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
return false;
}

/**
*Fecha a conexão enviando os pacotes restantes...
*Neste estado, qualquer chamada para os métodos sendReliable e sendUnreliable devem falhar.
**/
bool gpp_peer::disconnect()
{
profiler_snap();
unique_lock<shared_mutex> lck(mtx_con);
if((hcon==NULL)||(peer_id==0))
{
return false;
}
if(hcon->disconnectPeer(peer_id, GMODE_SOFTLY))
{
hstate.store(PEER_DISCONNECTING);
return true;
}
return false;
}

/**
*Fecha a conexão imediatamente.
*Quaisquer pacotes pendentes serão perdidos.
**/
bool gpp_peer::disconnectNow()
{
profiler_snap();
unique_lock<shared_mutex> lck(mtx_con);
if((hcon==NULL)||(peer_id==0))
{
return false;
}
if(hcon->disconnectPeer(peer_id, GMODE_NOW))
{
hstate.store(PEER_DISCONNECTED);
return true;
}
return false;
}

/**
*Faça uma solicitação ping.
*Isto normalmente é usado pelo cliente, mas também pode ser usado pelo servidor, apesar de não fazer muito sentido.
*O ping é dado como morto a cima de 10 segundos. Ou seja, enquanto o ping estiver ativo, outra solicitação não pode ser feita.
**/
void gpp_peer::makePing()
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

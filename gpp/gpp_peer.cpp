
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
#include"debug_system.h"
#include"SafeFlags.h"
#include"packet.h"
#include"event.h"
#include"EventQueue.h"
#include"gpp_networkinterface.h"
#include"gpp_peer.h"

using namespace std;

namespace gpp
{
gpp_peer::gpp_peer()
{
peer_id=0;
connection_time=0;
hState.setState(PEER_DEFAULT);
hState.replaceFlags(0);
this->hcon=hcon;
onping.store(0);
}

gpp_peer::~gpp_peer()
{
hcon=NULL;
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
bool gpp_peer::sendUnreliable(const string& data)
{
unique_lock<shared_mutex> lck(mtx_con);
if((hcon==NULL)||(peer_id==0))
{
return false;
}
uint32 x=hState.getState();
switch(x)
{
case PEER_ALT:
case PEER_CONNECTED:
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
bool gpp_peer::sendReliable(const string& data)
{
unique_lock<shared_mutex> lck(mtx_con);
if((hcon==NULL)||(peer_id==0))
{
return false;
}
uint32 x=hState.getState();
switch(x)
{
case PEER_ALT:
case PEER_CONNECTED:
{
return hcon->sendReliable(peer_id, data)>0;
}
}
return false;
}

/**
*Cria um pacote e tenta enviar de forma não confiável pela internet.
**/
bool gpp_peer::sendPacketUnreliable(uint32 type, uint32 flags, int64 timeout, const string& data)
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
bool gpp_peer::sendPacketReliable(uint32 type, uint32 flags, int64 timeout, const string& data)
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

void gpp_peer::sendAnswerSuccess(const std::string& cmd, const std::string& data)
{
if(isOnServer())
{
packet pack;
pack.type=PACKET_ANSWER;
pack.command=cmd;
pack.data=data;
pack.errorcode=0;
pack.errormsg="sucesso";
sendReliable(pack.serialize());
}
}

void gpp_peer::sendAnswerError(const std::string& cmd, uint32 errorcode, const std::string& msg)
{
if(isOnServer())
{
packet pack;
pack.type=PACKET_ANSWER;
pack.command=cmd;
pack.data="";
pack.errorcode=errorcode;
pack.errormsg=msg;
sendReliable(pack.serialize());
}
}

/**
*Fecha a conexão enviando os pacotes restantes...
*Neste estado, qualquer chamada para os métodos sendReliable e sendUnreliable devem falhar.
**/
bool gpp_peer::disconnect()
{
unique_lock<shared_mutex> lck(mtx_con);
if((hcon==NULL)||(peer_id==0))
{
return false;
}
switch(this->getHState())
{
case PEER_ALT:
case PEER_CONNECTED:
{
if(hcon->disconnectPeer(peer_id, GMODE_SOFTLY))
{
hState.setState(PEER_DISCONNECTING);
return true;
}
break;
}
}
return false;
}

/**
*Fecha a conexão imediatamente.
*Quaisquer pacotes pendentes serão perdidos.
**/
bool gpp_peer::disconnectNow()
{
unique_lock<shared_mutex> lck(mtx_con);
if((hcon==NULL)||(peer_id==0))
{
return false;
}
switch(this->getHState())
{
case PEER_ALT:
case PEER_CONNECTED:
{
if(hcon->disconnectPeer(peer_id, GMODE_NOW))
{
hState.setState(PEER_DISCONNECTED);
return true;
}
break;
}
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
if(onping.load()>0)
{
return;
}
onping.store(get_timestamp_ms());
sendPacketReliable(PACKET_PING, 0, 0, "");
}
}

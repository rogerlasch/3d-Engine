
/**
*Escrito por Roger em setembro de 2022.
*Este arquivo faz parte da game programming platform.
*Classe de comunicação básica.
*Contém métodos para envio de mensagens entre o servidor e cliente.
**/
#ifndef GPP_PEER_H
#define GPP_PEER_H

namespace gpp
{

//Estados de conexão...
enum PEER_STATES
{
PEER_DEFAULT=0,//Padrão, quando não existe conexão aberta...
PEER_ALT,//Aguardando a autenticação...
PEER_CONNECTED,//conectado...
PEER_DISCONNECTING,//A conexão está em processo de desconexão...
PEER_DISCONNECTED//O par está desconectado...
};

//Identificação da conexão...
enum PEER_FLAGS
{
PEER_BOT=(1<<0),//A conexão está sendo gerenciada a um bot... O servidor não deve enviar alguns pacotes para um bot...
PEER_REGULAR_CLIENT=(1<<0),//A conexão pertence a um jogador real...
PEER_CLIENT=(1<<2),//O peer está sendo gerenciado pelo cliente...
PEER_SERVER=(1<<3)//O peer está sendo gerenciado pelo servidor.
};

class gpp_peer
{
protected:
uint32 peer_id;//Id da conexão...
std::atomic<uint32> hstate;//estado da conexão...
std::atomic<int64> onping;//Usado em solicitações ping...
SafeFlags<uint32> pflags;
int64 connection_time;//Timestamp da hora da conexão...
std::string peer_name;
gpp_networkinterface* hcon;//Objeto gerenciador de rede...
mutable std::shared_mutex mtx_con;
public:
gpp_peer(gpp_networkinterface* hcon);
gpp_peer(const gpp_peer& pr)=delete;
gpp_peer& operator=(const gpp_peer& pr)=delete;
virtual ~gpp_peer();
bool isBot()const;
bool isRegularClient()const;
bool isOnClient()const;
bool isOnServer()const;
bool isConnected()const;
bool isDisconnected()const;
bool isAlt()const;
uint32 getPFlags()const;
void setPFlags(uint32 pflags);
std::string getPeerName()const;
void setPeerName(const std::string& peer_name);
uint32 getPeerId()const;
void setPeerId(uint32 peer_id);
uint32 getHState()const;
void setHState(uint32 hstate);
int64 getConnectionTime()const;
void setConnectionTime(int64 connection_time);
gpp_networkinterface* getHcon()const;
void setHcon(gpp_networkinterface* hserver);
bool sendUnreliable(const std::string& data);
bool sendReliable(const std::string& data);
bool sendPacketUnreliable(uint32 type, uint32 flags, int64 timeout, const std::string& data);
bool sendPacketReliable(uint32 type, uint32 flags, int64 timeout, const std::string& data);
void sendAnswerSuccess(const std::string& cmd, const std::string& data);
void sendAnswerError(const std::string& cmd, uint32 errorcode, const std::string& msg);
bool disconnect();
bool disconnectNow();
void makePing();
};
}
#endif


/**
*Escrito por Roger em setembro de 2022.
*Este arquivo faz parte da game programming platform.
*Classe de comunicação básica.
*Contém métodos para envio de mensagens entre o servidor e cliente.
**/
#ifndef GPP_PEER_H
#define GPP_PEER_H

#include"StateManager.h"

namespace gpp
{

//Estados de conexão...
enum PEER_STATES
{
PEER_DEFAULT=0,//Padrão, quando não existe conexão aberta...
PEER_CONNECTING,//Tentando se conectar...
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
std::atomic<int64> onping;//Usado em solicitações ping...
int64 connection_time;//Timestamp da hora da conexão...
std::string peer_name;
gpp_networkinterface* hcon;//Objeto gerenciador de rede...
StateManager hState;
mutable std::shared_mutex mtx_con;
public:
gpp_peer();
gpp_peer(const gpp_peer& pr)=delete;
gpp_peer& operator=(const gpp_peer& pr)=delete;
virtual ~gpp_peer();
inline bool isBot()const { return hState.flag_contains(PEER_BOT); }
inline bool isRegularClient()const { return hState.flag_contains(PEER_REGULAR_CLIENT); }
inline bool isOnClient()const { return hState.flag_contains(PEER_CLIENT); }
inline bool isOnServer()const { return hState.flag_contains(PEER_SERVER); }
inline bool isConnected()const { return hState.getState()==PEER_CONNECTED; }
inline bool isDisconnected()const { return hState.getState()==PEER_DISCONNECTED; }
inline bool isAlt()const { return hState.getState()==PEER_ALT; }
inline uint32 getPeerFlags()const { return hState.getFlags(); }
inline void setPeerFlags(uint32 pflags) { this->hState.replaceFlags(pflags); }
inline std::string getPeerName()const { return this->peer_name; }
inline void setPeerName(const std::string& peer_name) { this->peer_name=peer_name; }
uint32  getHState()const { return hState.getState(); }
void  setHState(uint32 hstate) { this->hState.setState(hstate); }

uint32 getPeerId()const;
void setPeerId(uint32 peer_id);
int64 getConnectionTime()const;
void setConnectionTime(int64 connection_time);
gpp_networkinterface* getHcon()const;
void setHcon(gpp_networkinterface* hcon);

bool sendUnreliable(const std::string& data);
bool sendReliable(const std::string& data);
bool sendPacketUnreliable(uint32 type, uint32 flags, int64 timeout, const std::string& data);
bool sendPacketReliable(uint32 type, uint32 flags, int64 timeout, const std::string& data);
virtual void sendAnswerSuccess(const std::string& cmd, const std::string& data);
virtual void sendAnswerError(const std::string& cmd, uint32 errorcode, const std::string& msg);
bool disconnect();
bool disconnectNow();
void makePing();
};
}
#endif

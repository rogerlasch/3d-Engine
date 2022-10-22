
/**
*Escrito por Roger em setembro de 2022.
*Este arquivo faz parte da game programming platform.
*Classe de comunicação básica.
*Contém métodos para envio de mensagens entre o servidor e cliente.
**/
#ifndef BASIC_PEER_H
#define BASIC_PEER_H

class ISteamNetworkingSockets;
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
enum PEER_TYPES
{
PEER_UNKNOWN=0,//O tipo é desconhecido... Isso não é bom...
PEER_BOT,//A conexão está sendo gerenciada a um bot... O servidor não deve enviar alguns pacotes para um bot...
PEER_REGULAR_CLIENT//A conexão pertence a um jogador real...
};

class basic_peer
{
protected:
uint32 peer_id;//Id da conexão...
uint32 hclass;//A classe que este peer está associado. Cliente ou servidor...
ISteamNetworkingSockets* hserver;//Um handle para o objeto de conexão da steam...
std::atomic<uint32> hstate;//estado da conexão...
std::atomic<uint32> ctype;//Identidade da conexão, bot ou jogador real...
std::atomic<int64> onping;//Usado em solicitações ping...
int64 connection_time;//Timestamp da hora da conexão...
mutable std::shared_mutex mtx_con;
std::shared_ptr<PacketWaitable> hpack;//Usado para gerenciar os pacotes do tipo PACK_WAITABLE...
public:
basic_peer(uint32 ctype, uint32 hclass);
basic_peer(const basic_peer& pr)=delete;
basic_peer& operator=(const basic_peer& pr)=delete;
virtual ~basic_peer();
bool isBot()const;
bool isRegularClient()const;
bool isOnClient()const;
bool isOnServer()const;
bool isConnected()const;
bool isDisconnected()const;
bool isAlt()const;
uint32 getHclass()const;
uint32 getCtype()const;
void setCtype(uint32 ctype);
uint32 getPeerId()const;
void setPeerId(uint32 peer_id);
uint32 getHstate()const;
void setHstate(uint32 hstate);
int64 getConnectionTime()const;
void setConnectionTime(int64 connection_time);
ISteamNetworkingSockets* getHserver()const;
void setHserver(ISteamNetworkingSockets* hserver);
bool sendUnreliable(const std::string& data);
bool sendReliable(const std::string& data);
bool sendPacketUnreliable(uint32 type, uint32 flags, int64 timeout, const std::string& data);
bool sendPacketReliable(uint32 type, uint32 flags, int64 timeout, const std::string& data);
bool sendAndWait(uint32 type, const std::string& data, packet* result);
bool sendAnswerSuccess(const std::string& data);
bool sendAnswerError(const std::string& errormsg);
bool disconnect();
bool disconnectNow();
void makePing();
friend class basic_server;
};
}
#endif

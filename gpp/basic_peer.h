
/**
*Escrito por Roger em setembro de 2022.
*Este arquivo faz parte da game programming platform.
*Classe de comunica��o b�sica.
*Cont�m m�todos para envio de mensagens entre o servidor e cliente.
**/
#ifndef BASIC_PEER_H
#define BASIC_PEER_H

class ISteamNetworkingSockets;
namespace gpp
{

//Estados de conex�o...
enum PEER_STATES
{
PEER_DEFAULT=0,//Padr�o, quando n�o existe conex�o aberta...
PEER_ALT,//Aguardando a autentica��o...
PEER_CONNECTED,//conectado...
PEER_DISCONNECTING,//A conex�o est� em processo de desconex�o...
PEER_DISCONNECTED//O par est� desconectado...
};

//Identifica��o da conex�o...
enum PEER_TYPES
{
PEER_UNKNOWN=0,//O tipo � desconhecido... Isso n�o � bom...
PEER_BOT,//A conex�o est� sendo gerenciada a um bot... O servidor n�o deve enviar alguns pacotes para um bot...
PEER_REGULAR_CLIENT//A conex�o pertence a um jogador real...
};

class basic_peer
{
protected:
uint32 peer_id;//Id da conex�o...
uint32 hclass;//A classe que este peer est� associado. Cliente ou servidor...
ISteamNetworkingSockets* hserver;//Um handle para o objeto de conex�o da steam...
std::atomic<uint32> hstate;//estado da conex�o...
std::atomic<uint32> ctype;//Identidade da conex�o, bot ou jogador real...
std::atomic<int64> onping;//Usado em solicita��es ping...
int64 connection_time;//Timestamp da hora da conex�o...
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

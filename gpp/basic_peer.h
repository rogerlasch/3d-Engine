


#ifndef BASIC_PEER_H
#define BASIC_PEER_H

class ISteamNetworkingSockets;
namespace gpp
{

enum PEER_STATES
{
PEER_DEFAULT=0,//Padrão, quando não existe conexão aberta...
PEER_ALT,//Aguardando a autenticação...
PEER_CONNECTED,
PEER_DISCONNECTING,
PEER_DISCONNECTED
};

class basic_peer
{
protected:
uint32 peer_id;
ISteamNetworkingSockets* hserver;
std::atomic<uint32> hstate;
int64 connection_time;
mutable std::shared_mutex mtx_con;
public:
basic_peer();
basic_peer(const basic_peer& pr)=delete;
basic_peer& operator=(const basic_peer& pr)=delete;
virtual ~basic_peer();
bool isConnected()const;
bool isDisconnected()const;
bool isAlt()const;
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
bool disconnect();
bool disconnectNow();
void updateCon();
};
}
#endif




#ifndef BASIC_SERVER_H
#define BASIC_SERVER_H

namespace gpp
{

enum GPP_SERVER_STATES
{
SERVER_DEFAULT=0,
SERVER_STARTING,
SERVER_RUNNING,
SERVER_SHUTING_DOWN,
SERVER_FINISHED
};

#define GPP_SERVER_DEFAULT_PORT 4000
#define GPP_SERVER_DEFAULT_MAX_PLAYERS 100


class basic_server : public GppSteamBase
{
private:
std::atomic<uint32> hstate;
std::unordered_map<uint32, basic_peer*> connections;
uint32 listenport;
uint32 max_players;
uint32 listensock;
mutable std::shared_mutex mtx_con;
public:
basic_server();
basic_server(const basic_server& bs)=delete;
basic_server& operator=(const basic_server& bs)=delete;
virtual ~basic_server();
uint32 getPort()const;
uint32 getMaxPlayers()const;
uint32 getHstate()const;
bool start(uint32 port=GPP_SERVER_DEFAULT_PORT, uint32 max_players=GPP_SERVER_DEFAULT_MAX_PLAYERS);
bool shutdown();
bool run();
void update();
bool peerDisconnect(uint32 peer_id);
bool peerDisconnectNow(uint32 peer_id);
void peerDisconnectAll();
bool peerIsConnected(uint32 peer_id)const;
private:
virtual void connectionStatusCallback(SteamNetConnectionStatusChangedCallback_t* pinfo);
};
}
#endif

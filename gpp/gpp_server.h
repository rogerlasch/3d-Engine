


#ifndef GPP_SERVER_H
#define GPP_SERVER_H

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
#define GPP_SERVER_DEFAULT_MAX_PLAYERS 10000


class gpp_server : public EventQueue
{
protected:
uint32 listenport;
uint32 listensock;
uint32 max_players;
StateManager hState;
std::unordered_map<uint32, gpp_peer*> peers;
gpp_networkinterface* hcon;
public:
gpp_server();
gpp_server(const gpp_server& gs)=delete;
gpp_server& operator=(const gpp_server& gs)=delete;
virtual ~gpp_server();

inline uint16 getPort()const { return this->listenport; }
inline uint32 playerCount()const { return peers.size(); }
inline uint32 getMaxPlayers()const { return this->max_players; }
inline uint32 getHState()const { return hState.getState(); }
inline void setHState(uint32 hs) { this->hState.setState(hs); }
inline gpp_networkinterface* getHCon()const { return this->hcon; }
inline void setHcon(gpp_networkinterface* hcon) { this->hcon=hcon; }

virtual bool start(uint16 port, uint32 max_players);
virtual void shutdown();
gpp_peer* getPeer(uint32 peer_id)const;
virtual bool peerDisconnect(uint32 id);
virtual void peerDisconnectAll();
virtual void run();
virtual void update();
virtual void pollNet();
virtual void pollEvents();
virtual void processNetMessage(gpp_peer* peer, const std::string& msg);
virtual void dispatchEvent(Event* ev);
virtual void eventPostCallback(Event* ev);
};
}
#endif

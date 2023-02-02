


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
private:
uint32 listenport;
uint32 listensock;
uint32 max_players;
std::atomic<uint32> hstate;
std::unordered_map<uint32, gpp_peer*> peers;
gpp_networkinterface* hcon;
public:
gpp_server(gpp_networkinterface* hcon);
gpp_server(const gpp_server& gs)=delete;
gpp_server& operator=(const gpp_server& gs)=delete;
virtual ~gpp_server();
uint16 getPort()const;
uint32 getMaxPlayers()const;
uint32 playerCount()const;
uint32 getHState()const;
void setHState(uint32 hstate);
gpp_networkinterface* getHCon()const;
virtual bool start(uint16 port, uint32 max_players);
virtual void shutdown();
gpp_peer* getPeer(uint32 peer_id)const;
virtual void peerDisconnectAll();
virtual void run();
virtual void update();
virtual void pollNet();
virtual void pollEvents();
virtual void processNetMessage(gpp_peer* peer, const std::string& msg);
virtual void dispatchEvent(Event* ev);
private:
virtual void netCallBack(uint32 event, uint32 peer_id);
virtual gpp_peer* createNewPeer();
};
}
#endif

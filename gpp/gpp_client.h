

#ifndef GPP_CLIENT_H
#define GPP_CLIENT_H

namespace gpp
{

class gpp_client : public gpp_peer, public EventQueue
{
private:
uint16 port;
std::string ipaddress;
public:
gpp_client(uint32 pflags, gpp_networkinterface* hcon);
gpp_client(const gpp_client& hc)=delete;
gpp_client& operator=(const gpp_client& hc)=delete;
virtual ~gpp_client();
uint16 getPort()const;
std::string getIpAddress()const;
virtual bool connect(const std::string& ipaddress, uint16 port);
virtual void shutdown();
virtual void run();
virtual void update();
virtual void pollNet();
virtual void pollEvents();
virtual void processNetMessage(const std::string& msg);
virtual void dispatchEvent(Event* ev);
private:
virtual void netCallBack(uint32 event, uint32 peer_id);
};
}
#endif

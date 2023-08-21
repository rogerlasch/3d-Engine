

#ifndef GPP_CLIENT_H
#define GPP_CLIENT_H

namespace gpp
{

class gpp_client : public gpp_peer, public EventQueue
{
private:
uint16 port;
std::string ipaddress;
std::unordered_map<std::string, std::pair<int64, packet*>> answers;
public:
gpp_client();
gpp_client(const gpp_client& hc)=delete;
gpp_client& operator=(const gpp_client& hc)=delete;
virtual ~gpp_client();
uint16 getPort()const;
std::string getIpAddress()const;
virtual bool connect(const std::string& ipaddress, uint16 port);
virtual bool altConnection();
virtual void shutdown();
virtual void run();
virtual void update();
virtual void pollNet();
virtual void pollEvents();
virtual void pollAnswers();
virtual void pushAnswer(packet* hpack);
virtual bool getAnswer(const std::string& cmd, packet** hpack);
virtual void processNetMessage(const std::string& msg);
virtual void dispatchEvent(Event* ev);
virtual void eventPostCallback(Event* ev);
};
}
#endif

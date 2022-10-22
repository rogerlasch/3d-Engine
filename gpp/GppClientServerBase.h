

#ifndef GPP_CLIENTSERVERBASE_H
#define GPP_CLIENTSERVERBASE_H

namespace gpp
{

enum GNET_TYPES
{
GNET_UNKNOWN=0,
GNET_CLIENT,
GNET_SERVER
};

enum GNET_FLAGS
{
GF_PACKAGED_MESSAGE=(1<<0),
GF_SIMPLE_MESSAGE=(1<<1),
};

class basic_peer;
class GppClientServerBase : public GppSteamBase, public EventQueue
{
private:
uint32 htype;
protected:
SafeFlags<uint32> hflags;
public:
GppClientServerBase(uint32 htype);
GppClientServerBase(const GppClientServerBase& c)=delete;
GppClientServerBase& operator=(const GppClientServerBase& c)=delete;
virtual ~GppClientServerBase();
uint32 getHtype()const;
bool isClient()const;
bool isServer()const;
virtual void run()=0;
virtual void update()=0;
virtual void pollNet()=0;
virtual void pollEvents()=0;
virtual void processSimpleNetMessage(basic_peer* peer, const std::string& msg)=0;
virtual void processPackagedNetMessage(basic_peer* peer, const std::string& msg)=0;
virtual void dispatchEvent(event* ev)=0;
};
}
#endif




#ifndef GPP_NETWORKINTERFACE_H
#define GPP_NETWORKINTERFACE_H

namespace gpp
{
enum GPP_DISCONNECT_MODE
{
GMODE_NOW=0,
GMODE_SOFTLY
};

typedef std::function<void(uint32, uint32)> GPPCONNECTIONCALLBACK;
typedef std::string GMESSAGE;
typedef std::vector<GMESSAGE> GMESSAGELIST;
class gpp_networkinterface
{
protected:
GPPCONNECTIONCALLBACK hcall;
public:
GPPCONNECTIONCALLBACK getConnectionCallBack()const { return this->hcall;}
void setConnectionCallBack(GPPCONNECTIONCALLBACK hcall) { this->hcall=hcall;}
virtual uint32 connectToServer(const std::string& address, uint16 port, GPPCONNECTIONCALLBACK hcall)=0;
virtual uint32 createListenSocket(uint16 port, GPPCONNECTIONCALLBACK hcall)=0;
virtual uint32 closeListenSocket(uint32 sock)=0;
virtual uint32 sendReliable(uint32 peer_id, const GMESSAGE& msg)=0;
virtual uint32 sendUnreliable(uint32 peer_id, const GMESSAGE& msg)=0;
virtual uint32 receiveMessage(uint32 peer_id, GMESSAGE& msg)=0;
virtual uint32 receiveMessages(uint32 peer_id, GMESSAGELIST& msgs, uint32 n)=0;
virtual bool disconnectPeer(uint32 peer_id, uint32 gmode)=0;
virtual void onLoop()=0;
};
}
#endif

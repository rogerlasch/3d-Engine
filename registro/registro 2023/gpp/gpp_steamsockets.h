


#ifndef GPP_STEAMSOCKETS_H
#define GPP_STEAMSOCKETS_H

#include <GameNetworkingSockets/steam/steamnetworkingsockets.h>
#include <GameNetworkingSockets/steam/isteamnetworkingutils.h>

namespace gpp
{

class gpp_steamsockets : public gpp_networkinterface
{
private:
static std::atomic<uint32> refcount;
static std::mutex mtx_hinstance;
static gpp_steamsockets* hinstance;
public:
gpp_steamsockets();
gpp_steamsockets(const gpp_steamsockets& gp)=delete;
gpp_steamsockets& operator=(const gpp_steamsockets& gp)=delete;
virtual ~gpp_steamsockets();
virtual uint32 connectToServer(const std::string& address, uint16 port, EVENTPOSTCALLBACK hcall);
virtual uint32 createListenSocket(uint16 port, EVENTPOSTCALLBACK hcall);
virtual uint32 closeListenSocket(uint32 sock);
virtual uint32 sendReliable(uint32 peer_id, const GMESSAGE& msg);
virtual uint32 sendUnreliable(uint32 peer_id, const GMESSAGE& msg);
virtual uint32 receiveMessage(uint32 peer_id, GMESSAGE& msg);
virtual uint32 receiveMessages(uint32 peer_id, GMESSAGELIST& msgs, uint32 n);
virtual bool disconnectPeer(uint32 peer_id, uint32 gmode);
virtual void onLoop();
private:
void safeRunCallbacks();
void connectionStatusCallback_server(SteamNetConnectionStatusChangedCallback_t* pinfo);
static void staticConnectionStatusCallback_server(SteamNetConnectionStatusChangedCallback_t* pinfo);
void connectionStatusCallback_client(SteamNetConnectionStatusChangedCallback_t* pinfo);
static void staticConnectionStatusCallback_client(SteamNetConnectionStatusChangedCallback_t* pinfo);
};
}
#endif

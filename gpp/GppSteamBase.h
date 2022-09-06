

#ifndef GPP_STEAM_SOCKET_H
#define GPP_STEAM_SOCKET_H

namespace gpp
{

class GppSteamBase
{
protected:
ISteamNetworkingSockets* hsteam;
static GppSteamBase* hinstance;
static std::mutex mtx_hinstance;
static std::atomic<uint32> steamrefcount;
public:
GppSteamBase();
GppSteamBase(const GppSteamBase& c)=delete;
virtual ~GppSteamBase();
void safeRunCallbacks();
virtual void connectionStatusCallback(SteamNetConnectionStatusChangedCallback_t* pinfo);
static void staticConnectionStatusCallback(SteamNetConnectionStatusChangedCallback_t* pinfo);
};
}
#endif

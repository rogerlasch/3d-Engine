


#ifndef BASIC_CLIENT_H
#define BASIC_CLIENT_H

namespace gpp
{

class basic_client : public basic_peer, public GppSteamBase
{
private:
uint32 port;
std::string ipaddress;
public:
basic_client();
basic_client(const basic_client& bc)=delete;
basic_client& operator=(const basic_client& bc)=delete;
virtual ~basic_client();
uint32 getPort()const;
std::string getAddress()const;
bool connect(const std::string& address, uint32 port);
void run();
void update();
private:
virtual void connectionStatusCallback(SteamNetConnectionStatusChangedCallback_t* pinfo);
};
}
#endif

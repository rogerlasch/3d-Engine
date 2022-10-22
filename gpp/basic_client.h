

/**
*Escrito por Roger em setembro de 2022.
*Este arquivo faz parte da game programming platform.
*Classe de cliente básico. Será responsável por toda a comunicação com o servidor.
*Futuramente, os bots poderão herdar daqui e só implementarem sua IA.
**/
#ifndef BASIC_CLIENT_H
#define BASIC_CLIENT_H

namespace gpp
{

class basic_client : public basic_peer, public GppClientServerBase
{
private:
uint32 port;//A porta usada pela conexão...
std::string ipaddress;//Endereço ip do servidor...
public:
basic_client(uint32 ctype=PEER_REGULAR_CLIENT);
basic_client(const basic_client& bc)=delete;
basic_client& operator=(const basic_client& bc)=delete;
virtual ~basic_client();
uint32 getPort()const;
std::string getAddress()const;
bool connect(const std::string& address, uint32 port);
virtual void run();
virtual void update();
virtual void pollNet();
virtual void pollEvents();
virtual void processSimpleNetMessage(basic_peer* peer, const std::string& msg);
virtual void processPackagedNetMessage(basic_peer* peer, const std::string& msg);
virtual void dispatchEvent(event* ev);
private:
virtual bool altConnection();
virtual void connectionStatusCallback(SteamNetConnectionStatusChangedCallback_t* pinfo);
};
}
#endif

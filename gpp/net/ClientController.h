

#ifndef CLIENTCONTROLLER_H
#define CLIENTCONTROLLER_H

namespace gpp {

class ClientController : public NetObject {
protected:
    std::string address;
    uint16 port;            // Porta do servidor

public:
    ClientController();
    virtual ~ClientController();

    virtual bool connect(const std::string& address, uint16 port);
    virtual void disconnect();
    virtual void update() override;
    virtual void shutdown() override;
virtual void send(const std::string& data, bool reliable=true, uint32 priority=0);
virtual void sendPacket(Packet* hpack, bool reliable=true, uint32 priority=0);

uint16 getPort()const{return port;}
std::string getAddress()const{return address;}
virtual inline int32 getPingTime()const{return -1;}
void waitForConnection();
};
}  // namespace gpp
#endif  // CLIENTCONTROLLER_H

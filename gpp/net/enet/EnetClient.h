

#ifndef ENETCLIENT_H
#define ENETCLIENT_H

namespace gpp {

class EnetClient : public ClientController{
private:
ENetHost* hclient;
ENetPeer* hpeer;
public:
    EnetClient();
    virtual ~EnetClient();

    virtual bool connect(const std::string& address, uint16 port);
    virtual void disconnect();
    virtual void update() override;
    virtual void shutdown() override;
virtual void send(const std::string& data, bool reliable=true, uint32 priority=0);
virtual int32 getPingTime()const;
};
}  // namespace gpp
#endif  // ENETCLIENT_H



#ifndef ENETSERVER_H
#define ENETSERVER_H


namespace gpp {

class EnetServer : public ServerController{
ENetHost* hserver;
public:
    EnetServer();
    virtual ~EnetServer();

    virtual bool listen(uint16 port);  // Inicia a escuta do servidor
    virtual void sendTo(uint32 id, const std::string& data, bool reliable, uint32 priority = 0);  // Envia dados para um cliente
    virtual void broadcast(const std::string& data, bool reliable, uint32 priority = 0);  // Envia dados para todos os clientes
    virtual bool disconnect(uint32 id);  // Desconecta um cliente
    virtual void disconnect_all();  // Desconecta todos os clientes
    virtual void update() override;
    virtual void shutdown() override;
virtual int32 getPingTime(uint32 id)const;

};
}  // namespace gpp
#endif  // ENETSERVER_H

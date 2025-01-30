

#ifndef SERVERCONTROLLER_H
#define SERVERCONTROLLER_H

#include <unordered_map>
#include <string>
#include <atomic>

namespace gpp {

class PeerInfo;
class ServerController : public NetObject {
protected:
    uint16 port;  // Porta na qual o servidor está escutando
std::unordered_set<uint32> pendingDeletions;
    std::unordered_map<uint32, PeerInfo*> clients;  // Mapeamento de IDs para informações dos clientes
    std::atomic<uint32> clientCounter;  // Contador para gerar IDs de clientes

public:
    ServerController();
    virtual ~ServerController();

    // Funções inline para verificar e obter informações de um cliente (peer)
    inline bool hasPeer(uint32 id) const { return clients.find(id) != clients.end(); }
inline uint32 clientCount()const{ return clients.size(); }
    inline PeerInfo* getPeerInfo(uint32 id) const {
        auto it = clients.find(id);
        return (it == clients.end()) ? nullptr : it->second;
    }

    // Funções de controle do servidor
    virtual bool listen(uint16 port);  // Inicia a escuta do servidor
    virtual void sendTo(uint32 id, const std::string& data, bool reliable, uint32 priority = 0);  // Envia dados para um cliente
virtual void sendTo(uint32 id, Packet* hpack, bool reliable=true, uint32 priority=0);
    virtual void broadcast(const std::string& data, bool reliable, uint32 priority = 0);  // Envia dados para todos os clientes
    virtual void broadcast(Packet* hpack, bool reliable, uint32 priority = 0);  // Envia dados para todos os clientes
    virtual bool disconnect(uint32 id);  // Desconecta um cliente
    virtual void disconnect_all();  // Desconecta todos os clientes

    // Atualização e encerramento
    virtual void update() override;
    virtual void shutdown() override;
virtual int32 getPingTime(uint32 id)const;
PeerInfo* insertPeerInfo(uint32 id, void* data);
bool removePeerInfo(uint32 id);
void cleanupPendings();

};
}  // namespace gpp
#endif  // SERVERCONTROLLER_H

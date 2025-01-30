
#ifndef PEERINFO_H
#define PEERINFO_H

#include <string>
#include <cstdint>
#include<atomic>

namespace gpp {

class ServerController;  // Forward declaration

class PeerInfo {
protected:
    uint32 id = 0;              // Identificador único do cliente
    uint16 port = 0;            // Porta do cliente
    std::string ipAddress;        // Endereço IP do cliente
    ServerController* hserver = nullptr; // Ponteiro para o servidor associado
    void* internalData = nullptr;         // Dados adicionais opcionais
void* userData=NULL;
std::atomic<uint32> refcount;
public:
    PeerInfo() = default;
    PeerInfo(const PeerInfo& info) = delete;  // Impede cópia
    PeerInfo& operator=(const PeerInfo& info) = delete;  // Impede atribuição
    virtual ~PeerInfo() = default;

    // Getters e Setters
    inline void setId(uint32 id) { this->id = id; }
    inline uint32 getId() const { return id; }

    inline void setPort(uint16 port) { this->port = port; }
    inline uint16 getPort() const { return port; }

    inline void setIpAddress(const std::string& ipAddress) { this->ipAddress = ipAddress; }
    inline const std::string& getIpAddress() const { return ipAddress; }

    inline void setServer(ServerController* server) { this->hserver = server; }
    inline ServerController* getServer() const { return hserver; }

    inline void setInternalData(void* d) { this->internalData= d; }
    inline void* getInternalData() const { return internalData; }

void setUserData(void* d){this->userData=d;}
void* getUserData()const{return userData;}

    // Método de envio de dados para o cliente
    inline void send(const std::string& data, bool reliable = true, uint32 priority = 0) {
        if (hserver != nullptr) {
            hserver->sendTo(id, data, reliable, priority);
        }
    }

    inline void sendPacket(Packet* hpack, bool reliable = true, uint32 priority = 0) {
        if (hserver != nullptr) {
            hserver->sendTo(id, hpack, reliable, priority);
        }
    }

inline uint32 getRefCount()const{return refcount.load();}
inline void adopt(){refcount.fetch_add(1);}
inline void release(){
if(refcount.load()>0){
refcount.fetch_sub(1);
}
}
};
}  // namespace gpp
#endif  // PEERINFO_H

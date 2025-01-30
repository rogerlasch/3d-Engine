

#ifndef NETBASE_H
#define NETBASE_H

#include <atomic>
#include <mutex>
#include <thread>
#include<vector>
#include<string>
#include "NetObject.h"

namespace gpp {

class ClientController;  // Forward declaration
class ServerController;

class NetBase {
protected:
    std::atomic<uint32_t> hstate;          // Armazena o estado atual da rede
    std::vector<NetObject*> hnets;  // Conjunto de objetos de rede (clientes e servidores)
    mutable std::mutex mtx;                // Mutex para proteger o acesso aos objetos de rede
    std::jthread runner;                   // Thread para rodar o ciclo de update
    std::condition_variable cv;            // Condição usada para sincronização

public:
    NetBase();
    NetBase(const NetBase& nt) = delete;
    NetBase& operator=(const NetBase& nt) = delete;
    virtual ~NetBase();

    virtual ClientController* createNewClient(NETEVENTCALLBACK hcall);
    virtual ServerController* createNewServer(NETEVENTCALLBACK hcall);
    virtual void loadDriver(){}
    virtual void unloadDriver(){}
    virtual void update();

        inline uint32_t getState() const { return hstate.load(); }

    void start();
    void stop();
    void waitStop();
        void stop_all();
};
}  // namespace gpp
#endif

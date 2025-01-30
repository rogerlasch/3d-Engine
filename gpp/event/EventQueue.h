
#ifndef EVENTQUEUE_H
#define EVENTQUEUE_H

#include <queue>
#include <unordered_map>
#include <vector>
#include <functional>
#include <shared_mutex>
#include <thread>
#include <atomic>
#include <condition_variable>
#include "Event.h"
#ifndef GPP_MEMORYPOOL_H
#include"../MemoryPool.h"
#endif

namespace gpp {

typedef std::function<void(Event*)> EVENTCALLBACK;

class EventQueue {
private:
    std::queue<Event*> events;                       // Fila de eventos
    EVENTCALLBACK hcall;                             // Callback global
    std::unordered_map<uint32, EVENTCALLBACK> hcallbacks; // Callbacks específicos
MemoryPool<Event> hpool;
    std::vector<std::jthread> threads;               // Threads para processamento
    mutable std::mutex mtx;                          // Proteção para a fila de eventos
    mutable std::mutex mtx_threads;                  // Proteção para o vetor de threads
    mutable std::shared_mutex mtx_callback;          // Proteção para os callbacks
    std::condition_variable cv;                      // Para gerenciar threads em espera
    std::atomic<uint32_t> eventCount;                // Contador de eventos na fila
    std::atomic<bool> running;                       // Estado da fila (ativa ou parada)
    ProfilerManager* hprofiler;                      // Gerenciador de profiling

public:
    EventQueue();
    EventQueue(const EventQueue& ev) = delete;
    virtual ~EventQueue();

    uint32 size() const;                             // Retorna o tamanho da fila
void enablePool(bool b){hpool.enablePool(b);}
    void cleanup();                                  // Limpa todos os eventos

    void setEventCallback(EVENTCALLBACK hcall);      // Define o callback global
    EVENTCALLBACK getEventCallback() const;          // Obtém o callback global
    void registerEventCallback(uint32 type, EVENTCALLBACK hev); // Registra callback por tipo
    void removeEventCallback(uint32 type);           // Remove callback por tipo
    EVENTCALLBACK getEventCallback(uint32 type);     // Obtém callback específico ou global

    Event* getNewEvent();                            // Obtém um novo evento do pool
    void pushEvent(uint32 type, const std::string& data, uint32 v1 = 0, uint32 v2 = 0, uint32 v3 = 0); // Adiciona evento na fila
    void pushEvent(Event* ev);                       // Adiciona evento específico na fila
    Event* getNextEvent();                           // Obtém o próximo evento da fila

    void start(uint32 threadCount=0);                                    // Inicia as threads de processamento
    void stop();                                     // Para as threads de processamento
    void pollEvents();                               // Processa eventos na fila
};

} // namespace gpp

#endif // EVENTQUEUE_H

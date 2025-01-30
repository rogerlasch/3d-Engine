
#ifndef NETEVENT_H
#define NETEVENT_H

#include <functional>
#include <string>

namespace gpp {

class NetEvent {
public:
    uint32 type;       // Tipo do evento (usando NetEventType)
    uint32 id;     // ID do peer relacionado ao evento
    std::string data;    // Dados associados ao evento

    // Construtores
    NetEvent();
    NetEvent(uint32 type, uint32 id, const std::string& data);
    NetEvent(const NetEvent& nt);

    // Atribuição
    NetEvent& operator=(const NetEvent& nt);

    // Destrutor virtual
    virtual ~NetEvent() = default;

std::string toString()const;
    // Método de limpeza
    virtual void cleanup();
};

// Definição do callback para eventos de rede
typedef std::function<void(NetEvent*)> NETEVENTCALLBACK;
}  // namespace gpp
#endif  // NETEVENT_H

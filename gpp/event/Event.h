
#ifndef GPP_EVENT_H
#define GPP_EVENT_H

#include <string>

namespace gpp {

class Event {
public:
    uint32 type;      // Tipo do evento
    uint32 v1;        // Valor 1
    uint32 v2;        // Valor 2
    uint32 v3;        // Valor 3
    int64 timestamp;  // Timestamp do evento
    std::string data;   // Dados adicionais

    Event();
    Event(const Event& ev) = delete;            // Copiar não é permitido
    Event& operator=(const Event& ev) = delete; // Atribuição não é permitida
    virtual ~Event();

    virtual std::string toString() const; // Gera uma representação do evento como string
    virtual void cleanup();               // Reseta os valores da classe
};
} // namespace gpp
#endif // GPP_EVENT_H

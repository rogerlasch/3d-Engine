
#ifndef CLOCKFRAME_H
#define CLOCKFRAME_H

#include <unordered_map>
#include <unordered_set>
#include <cstdint>
#include"EventFrame.h"

namespace gpp {

class ClockFrame {
private:
    uint32 currentFrame;
    uint32 eventCounter;  // Gerar IDs para eventos.
    std::unordered_set<uint32> hremoves;  // Eventos marcados para exclusão.
    std::unordered_map<uint32, EventFrame*> events;  // Tabela de eventos.

public:
    ClockFrame();
    ClockFrame(const ClockFrame& cf) = delete;
    ClockFrame& operator=(const ClockFrame& cf) = delete;
    ~ClockFrame();

    // Retorna o número de eventos ativos
    uint32 size() const;

uint32 pushEvent(uint32 hframes, EVENTFRAMECALLBACK hcall, void* userData=NULL, void* v1=NULL, void* v2=NULL);
uint32 pushClock(uint32 hframes, EVENTFRAMECALLBACK hcall, void* userData=NULL, void* v1=NULL, void* v2=NULL);
    // Adiciona um evento e retorna o ID gerado
    uint32 pushEvent(EventFrame* ev);

    // Remove um evento pelo ID
    bool removeEvent(uint32 id);

    // Remove todos os eventos
    void removeAll();

    // Atualiza o estado de todos os eventos e retorna o número de eventos processados
    uint32 update();

private:
    // Processa um evento individual
    void processEvent(EventFrame* ev);

    // Marca um evento para remoção
    void bindToRemove(uint32 id);

    // Remove eventos inativos
    void removeInactives();
};
} // namespace gpp
#endif

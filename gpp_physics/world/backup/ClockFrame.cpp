
#include<gpp/debug_system.h>
#include<functional>
#include"../types.h"
#include "ClockFrame.h"

using namespace std;

namespace gpp {

ClockFrame::ClockFrame() : currentFrame(0), eventCounter(0) {}

ClockFrame::~ClockFrame() {
    removeAll();  // Garante que todos os eventos sejam removidos ao destruir o objeto
}

// Retorna o número de eventos ativos
uint32 ClockFrame::size() const {
    return events.size();
}

uint32 ClockFrame::pushEvent(uint32 hframes, EVENTFRAMECALLBACK hcall, void* userData, void* v1, void* v2){
EventFrame ev;
ev.counter=0;
ev.frequence=hframes;
ev.hcall=hcall;
ev.userdata = userData;
ev.v2=v2;
return pushEvent(&ev);
}

// Adiciona um evento à lista de eventos e retorna o ID gerado para ele
uint32 ClockFrame::pushEvent(EventFrame* ev) {
eventCounter++;
    uint32 id =eventCounter;
    events[id] = ev->clone();
events[id]->id=id;
    return id;
}

// Remove um evento pelo seu ID
bool ClockFrame::removeEvent(uint32 id) {
    if (events.find(id) != events.end()) {
        bindToRemove(id);
        return true;
    }
    return false;
}

// Remove todos os eventos
void ClockFrame::removeAll() {
    for (auto& it : events) {
        delete it.second;
    }
    events.clear();
    hremoves.clear();
}

// Atualiza todos os eventos registrados
uint32 ClockFrame::update() {
    currentFrame++;
    for (auto& it : events) {
        processEvent(it.second);
    }
    removeInactives();  // Remove eventos que foram marcados para exclusão
    return currentFrame;
}

// Processa um evento individual
void ClockFrame::processEvent(EventFrame* ev) {
    if (ev) {
ev->counter++;
if(ev->counter>=ev->frequence){
if(ev->hcall){
ev->hcall(ev);
ev->counter=0;
if(ev->oneFire){
bindToRemove(ev->id);
}
}else{
bindToRemove(ev->id);
}
        }
    }
}

// Marca um evento para ser removido após a atualização
void ClockFrame::bindToRemove(uint32 id) {
    hremoves.insert(id);
}

// Remove eventos que foram marcados como inativos (no set `hremoves`)
void ClockFrame::removeInactives() {
    for (auto& id : hremoves) {
        auto it = events.find(id);
        if (it != events.end()) {
            delete it->second;
            events.erase(it);
        }
    }
    hremoves.clear();  // Limpa o set de eventos para exclusão
}
} // namespace gpp

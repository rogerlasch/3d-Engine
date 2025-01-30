
#ifndef GPP_ITASK_H
#define GPP_ITASK_H

#include <atomic>

namespace gpp {

enum ITASK_STATES {
    TS_DEFAULT = 0,
    TS_RUNNING,
    TS_REQUESTINGCANCEL,
    TS_CANCELED,
    TS_FINISHED
};

class iTask {
    std::atomic<uint32_t> hstate;  // Usar std::atomic para garantir a segurança em operações multi-thread
bool  isasync;
public:
    iTask() : hstate(TS_DEFAULT) {}  // Inicializando hstate com o estado padrão

    virtual ~iTask() = default;

    // Método virtual que será implementado por classes derivadas, executa a tarefa
    virtual void run() = 0;

    // Método virtual que será chamado quando a tarefa for finalizada
    virtual void end() = 0;

void setIsAsync(bool s){ this->isasync=s;}
bool getIsAsync()const{return isasync;}

    // Define o estado da tarefa, thread-safe por ser std::atomic
    virtual void setState(uint32_t newState) {
        hstate.store(newState);
    }

    // Retorna o estado atual da tarefa
    virtual uint32_t getState() const {
        return hstate.load();
    }

    // Solicita o cancelamento da tarefa, mudando o estado para TS_REQUESTINGCANCEL
    virtual void requestCancel() {
        setState(TS_REQUESTINGCANCEL);
    }
};

}  // namespace gpp

#endif // GPP_ITASK_H

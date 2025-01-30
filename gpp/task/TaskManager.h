
#ifndef GPP_TASKMANAGER_H
#define GPP_TASKMANAGER_H

#include <vector>
#include <memory>
#include <thread>
#include <mutex>
#include <queue>
#include <functional>
#include <atomic>
#include <condition_variable>
#include "iTask.h"

namespace gpp {

class TaskManager {
private:
    std::vector<std::thread> threads;              // Armazena os threads das tarefas ass�ncronas
    std::vector<std::shared_ptr<iTask>> tasks;     // Armazena todas as tarefas (sincronas e assincronas)
mutable     std::mutex taskMutex;                          // Garante acesso seguro �s tarefas
    std::atomic<bool> running;                     // Indica se o gerenciador est� ativo
    std::atomic<bool> asyncRunning;                // Indica se as tarefas ass�ncronas est�o sendo executadas
    bool enableAsync;                              // Define se o modo ass�ncrono est� habilitado
    std::condition_variable taskCV;                // Para sinalizar novas tarefas ou t�rmino
std::thread handle;
    void updateTasks();                            // Atualiza o estado das tarefas, executando as que est�o prontas
    void runTask(std::shared_ptr<iTask> task);     // Executa uma tarefa espec�fica

public:
    TaskManager(bool async = true);                // Construtor define se o modo ass�ncrono est� habilitado
    ~TaskManager();                                // Destrutor garante que todas as threads sejam finalizadas

    void pushTask(std::shared_ptr<iTask> task);    // Adiciona uma nova tarefa ao gerenciador
    void pushTask(std::function<void()> lambda, bool isasync=false);   // Adiciona uma tarefa lambda
    void runTasks();                               // Executa as tarefas sincronamente
    void runTasksAsync();                          // Executa as tarefas assincronamente
    void stopTasks(bool wait = true);              // Para todas as tarefas e opcionalmente espera a finaliza��o
    void waitForCompletion();                      // Aguarda todas as tarefas (sync e async) serem conclu�das

//private:
    bool tasksPending() const;                     // Verifica se ainda h� tarefas pendentes
};

} // namespace gpp
#endif // GPP_TASKMANAGER_H

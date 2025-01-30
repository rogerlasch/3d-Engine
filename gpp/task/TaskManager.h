
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
    std::vector<std::thread> threads;              // Armazena os threads das tarefas assíncronas
    std::vector<std::shared_ptr<iTask>> tasks;     // Armazena todas as tarefas (sincronas e assincronas)
mutable     std::mutex taskMutex;                          // Garante acesso seguro às tarefas
    std::atomic<bool> running;                     // Indica se o gerenciador está ativo
    std::atomic<bool> asyncRunning;                // Indica se as tarefas assíncronas estão sendo executadas
    bool enableAsync;                              // Define se o modo assíncrono está habilitado
    std::condition_variable taskCV;                // Para sinalizar novas tarefas ou término
std::thread handle;
    void updateTasks();                            // Atualiza o estado das tarefas, executando as que estão prontas
    void runTask(std::shared_ptr<iTask> task);     // Executa uma tarefa específica

public:
    TaskManager(bool async = true);                // Construtor define se o modo assíncrono está habilitado
    ~TaskManager();                                // Destrutor garante que todas as threads sejam finalizadas

    void pushTask(std::shared_ptr<iTask> task);    // Adiciona uma nova tarefa ao gerenciador
    void pushTask(std::function<void()> lambda, bool isasync=false);   // Adiciona uma tarefa lambda
    void runTasks();                               // Executa as tarefas sincronamente
    void runTasksAsync();                          // Executa as tarefas assincronamente
    void stopTasks(bool wait = true);              // Para todas as tarefas e opcionalmente espera a finalização
    void waitForCompletion();                      // Aguarda todas as tarefas (sync e async) serem concluídas

//private:
    bool tasksPending() const;                     // Verifica se ainda há tarefas pendentes
};

} // namespace gpp
#endif // GPP_TASKMANAGER_H

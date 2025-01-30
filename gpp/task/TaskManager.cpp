
#include<iostream>
#include<algorithm>
#include"../debug_system.h"
#include "TaskManager.h"

namespace gpp {

// Construtor: Define o modo assíncrono (por padrão, true)
TaskManager::TaskManager(bool async)
    : enableAsync(async), running(true), asyncRunning(false) {}

// Destrutor: Aguarda todas as threads assincronas finalizarem
TaskManager::~TaskManager() {
    stopTasks(true);
}

// Método para adicionar uma nova tarefa ao gerenciador
void TaskManager::pushTask(std::shared_ptr<iTask> task) {
    std::lock_guard<std::mutex> lock(taskMutex);

    // Se o modo assíncrono estiver ativado, ajusta a tarefa para ser executada assincronamente
    if (!enableAsync) {
        task->setIsAsync(false);
    }

    tasks.push_back(task);
    taskCV.notify_one();  // Notifica que uma nova tarefa foi adicionada
}

// Método para adicionar uma função lambda como tarefa
void TaskManager::pushTask(std::function<void()> lambda, bool isasync) {
    struct LambdaTask : public iTask {
        std::function<void()> func;
        LambdaTask(std::function<void()> f, bool s) : func(f) {setIsAsync(s);}
        void run() override { func(); }
        void end() override { /* Limpeza pós-execução, se necessário */ }
    };

    pushTask(std::make_shared<LambdaTask>(lambda, isasync));
}

// Método que executa as tarefas sincronamente
void TaskManager::runTasks() {
    if (asyncRunning) return;  // Impede execução se o modo assíncrono estiver ativo
    updateTasks();
}

// Método que executa as tarefas assincronamente
void TaskManager::runTasksAsync() {
    if (asyncRunning || !running) return;  // Impede execução se já estiver rodando async

    asyncRunning = true;
this->handle=std::thread([this]() {
        while (tasksPending()) {
            updateTasks();
            std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Evita loop apertado
        }
        asyncRunning = false;  // Finaliza a execução assíncrona
    });
}

// Método que para todas as tarefas e opcionalmente aguarda sua finalização
void TaskManager::stopTasks(bool wait) {
    running = false;

    // Sinaliza cancelamento das tarefas
    std::lock_guard<std::mutex> lock(taskMutex);
    for (auto& task : tasks) {
        task->requestCancel();
    }

    // Se "wait" for true, aguarda a conclusão
    if (wait) {
        waitForCompletion();
    }
}

// Aguarda a finalização de todas as tarefas (sync e async)
void TaskManager::waitForCompletion() {

while(tasksPending()){
if(!asyncRunning){
updateTasks();
}

if(handle.joinable()){
handle.join();
}

    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

    threads.clear();

    std::lock_guard<std::mutex> lock(taskMutex);
    tasks.clear();
}

// Atualiza o estado das tarefas, executando as que estão prontas
void TaskManager::updateTasks() {
    std::lock_guard<std::mutex> lock(taskMutex);

std::sort(tasks.begin(), tasks.end(), [](const std::shared_ptr<iTask>& t1, const std::shared_ptr<iTask>& t2)->bool{return t1->getIsAsync();});

    for (auto& task : tasks) {
        if (task->getState() == TS_DEFAULT) {
            if (task->getIsAsync()) {
                threads.emplace_back([this, task]() {
                    task->setState(TS_RUNNING);
                    task->run();
                    task->setState(TS_FINISHED);
                    task->end();
                });
            } else {
                runTask(task);  // Executa tarefa síncrona
            }
        } else if (task->getState() == TS_FINISHED) {
            // Tarefa finalizada, ela pode ser removida
            task->end();
        }
    }

    // Limpa tarefas finalizadas
    tasks.erase(std::remove_if(tasks.begin(), tasks.end(),
                               [](const std::shared_ptr<iTask>& task) {
                                   return task->getState() == TS_FINISHED;
                               }),
                tasks.end());
}

// Executa uma tarefa específica (sincronamente)
void TaskManager::runTask(std::shared_ptr<iTask> task) {

    task->setState(TS_RUNNING);
    task->run();  // Executa a tarefa
    task->setState(TS_FINISHED);
    task->end();  // Chama o método de finalização
}

// Verifica se ainda há tarefas pendentes
bool TaskManager::tasksPending() const {
    std::lock_guard<std::mutex> lock(taskMutex);
if(tasks.size()==0){
}
    for (const auto& task : tasks) {
        if (task->getState() != TS_FINISHED) {
            return true;
        }
    }
    return false;
}

} // namespace gpp

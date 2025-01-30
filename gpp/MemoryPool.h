
#ifndef GPP_MEMORYPOOL_H
#define GPP_MEMORYPOOL_H

#include <queue>
#include <mutex>
#include <atomic>
#include <string>
#include <sstream>
#include"types.h"

namespace gpp{

template <class T>
class MemoryPool {
private:
    std::atomic<uint64> newObjects{0};     // Contador de objetos criados
    std::atomic<uint64> deletedObjects{0}; // Contador de objetos deletados
    bool usePool{true};                      // Indica se o pool está habilitado
    std::queue<T*> items;                    // Fila de objetos disponíveis
    std::mutex mtx;                          // Mutex para acesso seguro ao pool

public:
    explicit MemoryPool(uint32 initialItems = 32, bool usePool=true);
    MemoryPool(const MemoryPool& mp) = delete;
    MemoryPool& operator=(const MemoryPool& mp) = delete;
    ~MemoryPool();

    std::string toString() const;
    void cleanup();
void cleanupObjects();
    void reserve(uint32 n);
    void enablePool(bool b);
    bool isPoolEnabled() const;
    T* getNewObject();
    void releaseObject(T* obj);
};

// Implementação do construtor
template <class T>
MemoryPool<T>::MemoryPool(uint32 initialItems, bool usePool) {
this->usePool=usePool;
if(usePool){
    reserve(initialItems);
}
}

// Implementação do destrutor
template <class T>
MemoryPool<T>::~MemoryPool() {
    cleanup();
}

// Método para criar ou reutilizar um objeto do pool
template <class T>
T* MemoryPool<T>::getNewObject() {
    std::lock_guard<std::mutex> lock(mtx);
    if (usePool && !items.empty()) {
        T* obj = items.front();
        items.pop();
        return obj;
    }

    newObjects++;
    return new T(); // Cria um novo objeto se o pool estiver vazio ou desativado
}

// Método para liberar um objeto de volta ao pool
template <class T>
void MemoryPool<T>::releaseObject(T* obj) {
    if (!obj) return; // Evita adicionar objetos nulos
    std::lock_guard<std::mutex> lock(mtx);
    if (usePool) {
        items.push(obj);
    } else {
        deletedObjects++;
        delete obj; // Deleta o objeto se o pool estiver desativado
    }
}

// Método para limpar todo o pool
template <class T>
void MemoryPool<T>::cleanup() {
cleanupObjects();
newObjects.store(0);
deletedObjects.store(0);
}

// Método para limpar todos objetos do pool.
template <class T>
void MemoryPool<T>::cleanupObjects() {
    std::lock_guard<std::mutex> lock(mtx);
    while (!items.empty()) {
        T* obj = items.front();
        items.pop();
        delete obj;
        deletedObjects++;
    }
}

// Método para reservar espaço no pool
template <class T>
void MemoryPool<T>::reserve(uint32 n) {
if(n==0)return;

    std::lock_guard<std::mutex> lock(mtx);

    for (uint32 i = 0; i < n; ++i) {
        items.push(new T());
    }

    newObjects += n;
}

// Método para ativar ou desativar o uso do pool
template <class T>
void MemoryPool<T>::enablePool(bool b) {
    usePool = b;

if(!usePool){
cleanupObjects();
}
}

// Método para verificar se o pool está habilitado
template <class T>
bool MemoryPool<T>::isPoolEnabled() const {
    return usePool;
}

// Método para obter uma representação em string do estado do pool
template <class T>
std::string MemoryPool<T>::toString() const {
    std::ostringstream oss;
    oss << "MemoryPool Status:\n"
        << "  New Objects: " << newObjects.load() << "\n"
        << "  Deleted Objects: " << deletedObjects.load() << "\n"
        << "  Objects in Pool: " << items.size() << "\n"
        << "  Pool Enabled: " << (usePool ? "Yes" : "No") << "\n";
    return oss.str();
}
}
#endif // GPP_MEMORYPOOL_H

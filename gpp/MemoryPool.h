
#ifndef GPP_MEMORYPOOL_H
#define GPP_MEMORYPOOL_H

#include <queue>
#include <mutex>
#include <atomic>
#include <string>
#include <sstream>
#include "types.h"

namespace gpp {

template <class T>
class MemoryPool {
private:
     std::atomic<uint64> newObjects{0};
     std::atomic<uint64> deletedObjects{0};
    bool usePool{true};
    std::queue<T*> items;
    std::mutex mtx;

public:
    explicit MemoryPool(uint32 initialItems = 32, bool usePool = true);
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
void releaseObjects(const std::vector<T*>& objs);
};
}
#endif

#ifndef GPP_MEMORYPOOL_CPP
#define GPP_MEMORYPOOL_CPP

namespace gpp{
template <class T>
MemoryPool<T>::MemoryPool(uint32 initialItems, bool usePool) {
    this->usePool = usePool;
    if (usePool) {
        reserve(initialItems);
    }
}

template <class T>
MemoryPool<T>::~MemoryPool() {
    cleanup();
}

template <class T>
T* MemoryPool<T>::getNewObject() {
    std::lock_guard<std::mutex> lock(mtx);
    if (usePool && !items.empty()) {
        T* obj = items.front();
        items.pop();
        return obj;
    }

    newObjects++;
    return new T();
}

template <class T>
void MemoryPool<T>::releaseObject(T* obj) {
    if (!obj) return;
    std::lock_guard<std::mutex> lock(mtx);
    if (usePool) {
        items.push(obj);
    } else {
        deletedObjects++;
delete obj;
    }
}

template<class T>
void MemoryPool<T>::releaseObjects(const std::vector<T*>& objs){
if(objs.empty()) return;

    std::lock_guard<std::mutex> lock(mtx);
for(auto& it : objs){
    if (usePool) {
        items.push(it);
    } else {
        deletedObjects++;
delete it;
    }
}
}

template <class T>
void MemoryPool<T>::cleanup() {
    cleanupObjects();
    newObjects.store(0);
    deletedObjects.store(0);
}

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

template <class T>
void MemoryPool<T>::reserve(uint32 n) {
    if (n == 0) return;

    std::lock_guard<std::mutex> lock(mtx);
    for (uint32 i = 0; i < n; ++i) {
        items.push(new T());
    }
    newObjects += n;
}

template <class T>
void MemoryPool<T>::enablePool(bool b) {
    usePool = b;
    if (!usePool) {
        cleanupObjects();
    }
}

template <class T>
bool MemoryPool<T>::isPoolEnabled() const {
    return usePool;
}

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
#endif

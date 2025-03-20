
#ifndef EVENTQUEUE_H
#define EVENTQUEUE_H

#include <vector>
#include <functional>
#include <thread>
#include <atomic>
#include <condition_variable>
#include"types.h"
#ifndef GPP_MEMORYPOOL_H
#include"MemoryPool.h"
#endif

namespace gpp {

template<class T>
class alignas(64) EventQueue {
private:
    using EVENTCALLBACK = std::function<void(T*)>;

    struct alignas(64) EventBatch {
        std::vector<T*> events;
        EventBatch* next = NULL;
    };

    EventBatch* first = NULL;
    EventBatch* last = NULL;
    EVENTCALLBACK hcall;
    MemoryPool<T> hpool;
    MemoryPool<EventBatch> batchPool;
    std::vector<std::jthread> threads;
    mutable std::mutex mtx;
    mutable std::mutex mtx_threads;
    std::condition_variable cv;
alignas(64)     std::atomic<uint64> eventCount;
alignas(64)     std::atomic<bool> running;

public:
    EventQueue();
    EventQueue(const EventQueue& ev) = delete;
    virtual ~EventQueue();

    uint64 size() const;
    void enablePool(bool b);
    void cleanup();

    void setEventCallback(EVENTCALLBACK hcall);
    EVENTCALLBACK getEventCallback() const;

    T* getNewEvent();
    void pushEvent(T* ev);

    void start(uint32 threadCount = 0);
    void stop();
    void pollEvents();

private:

    EventBatch* getNextBatch();
};
}
#endif

#ifndef EVENTQUEUE2_CPP
#define EVENTQUEUE2_CPP

namespace gpp {

template<class T>
EventQueue<T>::EventQueue() {
    running.store(false, std::memory_order_relaxed);
    eventCount.store(0, std::memory_order_relaxed);
    enablePool(true);
    batchPool.enablePool(true);
    batchPool.reserve(1024);
}

template<class T>
EventQueue<T>::~EventQueue() {
    stop();
    cleanup();
    hpool.cleanup();
    batchPool.cleanup();
}

template<class T>
uint64 EventQueue<T>::size() const {
    return eventCount.load(std::memory_order_relaxed);
}

template<class T>
    void EventQueue<T>::enablePool(bool b) {
hpool.enablePool(b);
if(b){
hpool.cleanup();
hpool.reserve(64);
}
}

template<class T>
void EventQueue<T>::cleanup() {
    try {
        std::lock_guard<std::mutex> lck(mtx);

        while (first != NULL) {
            for (auto& it : first->events) {
                hpool.releaseObject(it);
            }
            first->events.clear();
            eventCount.store(0, std::memory_order_relaxed);
            EventBatch* ev = first;
            first = first->next;
            ev->next = NULL;
            batchPool.releaseObject(ev);
        }

        first = NULL;
        last = NULL;
eventCount.store(0);
    } catch (const std::exception& e) {
        //logger::info("Exception in {}.\nError: {}", __FUNCTION__, e.what());
    }
}

template<class T>
void EventQueue<T>::setEventCallback(EVENTCALLBACK hcall) {
    this->hcall = hcall;
}

template<class T>
typename EventQueue<T>::EVENTCALLBACK EventQueue<T>::getEventCallback() const {
    return hcall;
}

template<class T>
T* EventQueue<T>::getNewEvent() {
    return hpool.getNewObject();
}

template<class T>
void EventQueue<T>::pushEvent(T* ev) {
    try {
        std::lock_guard<std::mutex> lck(mtx);

        EventBatch* sf = NULL;
        if (first == NULL) {
            first = batchPool.getNewObject();
first->next=NULL;
            last = first;
            sf = first;
        } else if (last->events.size() < 25) {
            sf = last;
        } else {
            last->next = batchPool.getNewObject();
            last = last->next;
            last->next = NULL;
            sf = last;
        }

        sf->events.push_back(ev);
        ++eventCount;

        if (running.load(std::memory_order_relaxed)) {
            cv.notify_one();
        }
    } catch (const std::exception& e) {
        hpool.releaseObject(ev);
        //logger::info("Exception in {}.\nError: {}", __FUNCTION__, e.what());
    }
}

template<class T>
typename EventQueue<T>::EventBatch* EventQueue<T>::getNextBatch() {
    try {
        std::unique_lock<std::mutex> lck(mtx);

        if (eventCount.load(std::memory_order_relaxed) == 0 && running.load(std::memory_order_relaxed)) {
            cv.wait(lck, [this]() { return eventCount.load(std::memory_order_relaxed) > 0 || !running.load(std::memory_order_relaxed); });
            if (!running.load(std::memory_order_relaxed)) return NULL;
        }

        if (first == NULL) {
            return NULL;
        }

        EventBatch* sf = first;
        if (first == last) {
            first = NULL;
            last = NULL;
            eventCount.store(0, std::memory_order_relaxed);
        } else {
            first = first->next;
            eventCount.fetch_sub(sf->events.size(), std::memory_order_relaxed);
        }

sf->next=NULL;
        return sf;
    } catch (const std::exception& e) {
        //logger::info("Exception in {}.\nError: {}", __FUNCTION__, e.what());
    }
    return NULL;
}

template<class T>
void EventQueue<T>::start(uint32 threadCount) {
    std::lock_guard<std::mutex> lck_threads(mtx_threads);

    if (running.load(std::memory_order_relaxed)) {
        return;
    }

    running.store(true, std::memory_order_relaxed);

    if (threadCount == 0) {
        threadCount = std::thread::hardware_concurrency();
        threadCount = threadCount > 0 ? threadCount : 2;
    }

    for (uint32 i = 0; i < threadCount; ++i) {
        threads.emplace_back([this](std::stop_token stopToken) {
            while (!stopToken.stop_requested() && running.load(std::memory_order_relaxed)) {
                pollEvents();
            }
        });
    }

    //logger::info("Threads iniciadas: {}", threadCount);
}

template<class T>
void EventQueue<T>::stop() {
    std::lock_guard<std::mutex> lck_threads(mtx_threads);

    if (!running.load(std::memory_order_relaxed)) {
        return;
    }

    running.store(false, std::memory_order_relaxed);
    cv.notify_all();

    for (auto& th : threads) {
        th.request_stop();
        th.join();
    }
    threads.clear();
}

template<class T>
void EventQueue<T>::pollEvents() {
    EventBatch* bt = NULL;
    while ((bt = getNextBatch()) != NULL) {
        for (auto& it : bt->events) {
            if (hcall) hcall(it);
        }
hpool.releaseObjects(bt->events);
bt->events.clear();
        batchPool.releaseObject(bt);
    }
}
}
#endif

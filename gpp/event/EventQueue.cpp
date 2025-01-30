
#include "../debug_system.h"
#include "EventQueue.h"

using namespace std;

namespace gpp {

EventQueue::EventQueue() : running(false), eventCount(0) {
    hprofiler = new ProfilerManager(PF_NANO, "EventQueue profiler.txt");
}

EventQueue::~EventQueue() {
    stop(); // Garante que as threads parem antes de destruir
    cleanup();
    hpool.cleanup();
hprofiler->dumpToFile();
    delete hprofiler;
}

uint32 EventQueue::size() const {
    return eventCount.load();
}

void EventQueue::cleanup() {
    try {
//auto hf=hprofiler->createProfilerObject(__FUNCTION__);
//hf->start();

        lock_guard<mutex> lck(mtx);
        while (!events.empty()) {
            Event* ev = events.front();
            events.pop();
            hpool.releaseObject(ev);
        }
        eventCount.store(0);
    } catch (const exception& e) {
        logger::info("Exception in {}.\nError: {}", __FUNCTION__, e.what());
    }
}

void EventQueue::setEventCallback(EVENTCALLBACK hcall) {
    unique_lock<shared_mutex> lck(mtx_callback);
    this->hcall = hcall;
}

EVENTCALLBACK EventQueue::getEventCallback() const {
    shared_lock<shared_mutex> lck(mtx_callback);
    return hcall;
}

void EventQueue::registerEventCallback(uint32 type, EVENTCALLBACK hev) {
    unique_lock<shared_mutex> lck(mtx_callback);
    hcallbacks[type] = hev;
}

void EventQueue::removeEventCallback(uint32 type) {
    unique_lock<shared_mutex> lck(mtx_callback);
    hcallbacks.erase(type);
}

EVENTCALLBACK EventQueue::getEventCallback(uint32 type) {
    shared_lock<shared_mutex> lck(mtx_callback);
    auto it = hcallbacks.find(type);
    return (it == hcallbacks.end()) ? hcall : it->second;
}

Event* EventQueue::getNewEvent() {
    return hpool.getNewObject();
}

void EventQueue::pushEvent(uint32 type, const string& data, uint32 v1, uint32 v2, uint32 v3) {
    Event* ev = getNewEvent();
    ev->type = type;
    ev->data = data;
    ev->v1 = v1;
    ev->v2 = v2;
    ev->v3 = v3;
    ev->timestamp = chrono::system_clock::now().time_since_epoch().count();

    pushEvent(ev);
}

void EventQueue::pushEvent(Event* ev) {
    try {
            lock_guard<mutex> lck(mtx);
//auto hf=hprofiler->createProfilerObject(__FUNCTION__);
//hf->start();

            events.push(ev);
            ++eventCount;
//        logger::info("Evento adicionado. Total de eventos: {}", eventCount.load());

if(running.load()==true){
        cv.notify_one(); // Notifica uma thread em espera
}
    } catch (const exception& e) {
        hpool.releaseObject(ev);
        logger::info("Exception in {}.\nError: {}", __FUNCTION__, e.what());
    }
}

Event* EventQueue::getNextEvent() {
try{

unique_lock<mutex> lck(mtx);

if((eventCount.load()==0)&&(running.load()==true)){
cv.wait(lck, [this](){ return eventCount.load()>0||running.load()==false;});
if(running.load()==false)return NULL;
}

//auto hf=hprofiler->createProfilerObject(__FUNCTION__);
//hf->start();

    if ( events.empty()) {
        return nullptr;
    }

    Event* ev = events.front();
    events.pop();
    --eventCount;
//logger::info("Removido... {}", eventCount.load());
    return ev;
}catch(const exception& e){
logger::info("Exception in {}.\nError: {}", __FUNCTION__, e.what());
}
return NULL;
}

void EventQueue::start(uint32 threadCount) {
    lock_guard<mutex> lck_threads(mtx_threads);
//auto hf=hprofiler->createProfilerObject(__FUNCTION__);
//hf->start();

    if (running.load()==true) {
        return;
    }

running.store(true);

if(threadCount==0){
threadCount = thread::hardware_concurrency();
    threadCount = threadCount > 0 ? threadCount : 2;
}

    for (uint32 i = 0; i < threadCount; ++i) {
        threads.emplace_back([this](stop_token stopToken) {
            while (!stopToken.stop_requested() && running.load()) {
                pollEvents();
            }
        });
    }

    logger::info("Threads iniciadas: {}", threadCount);
}

void EventQueue::stop() {
        lock_guard<mutex> lck_threads(mtx_threads);
//auto hf=hprofiler->createProfilerObject(__FUNCTION__);
//hf->start();

        if (running.load()==false){
            return;
        }

running.store(false);
    cv.notify_all(); // Acorda todas as threads em espera

    for (auto& th : threads) {
        if (th.joinable()) {
            th.request_stop();
            th.join();
        }
    }
    threads.clear();
}

void EventQueue::pollEvents() {
//auto hf=hprofiler->createProfilerObject(__FUNCTION__);
//hf->start();

    Event* ev = nullptr;
    while ((ev = getNextEvent()) != nullptr) {
        EVENTCALLBACK evcall = getEventCallback(ev->type);
        if (evcall) {
            evcall(ev);
        }
        hpool.releaseObject(ev);
    }
}
} // namespace gpp


#include<atomic>
#include<functional>
#include<thread>
#include"debug_system.h"
#include"StateManager.h"

using namespace std;

namespace gpp{

StateManager::StateManager(uint32 st){
currentState.store(st);
lastState.store(st);
stage.store(0);
lastStage.store(0);
htime.store(get_timestamp_ms());
lastTime.store(htime.load());
}

StateManager::StateManager(const StateManager& st){
*this=st;
}

StateManager& StateManager::operator=(const StateManager& st){
this->lastState.store(st.lastState.load());
this->currentState.store(st.currentState.load());
this->lastStage.store(st.lastStage.load());
this->stage.store(st.stage.load());
this->lastTime.store(st.lastTime.load());
this->htime.store(st.htime.load());
this->flags.store(st.flags.load());
return *this;
}

StateManager::~StateManager() {}

void StateManager::setStage(uint32 s) {
    stage.store(s, std::memory_order_relaxed);
}

void StateManager::setState(uint32 s) {
    lastState.store(currentState.load(std::memory_order_relaxed), std::memory_order_relaxed);
    currentState.store(s, std::memory_order_relaxed);
lastStage.store(stage.load());
stage.store(0);
lastTime.store(htime.load());
    htime.store(get_timestamp_ms());
}

uint32 StateManager::getStage() const {
    return stage.load(std::memory_order_relaxed);
}

uint32 StateManager::getLastStage()const{
return lastStage.load();
}


uint32 StateManager::getState() const {
    return currentState.load(std::memory_order_relaxed);
}

uint32 StateManager::getLastState() const {
    return lastState.load(std::memory_order_relaxed);
}

int64 StateManager::getTime() const {
    return htime.load(std::memory_order_relaxed);
}

int64 StateManager::getLastTime()const{
return lastTime.load();
}

bool StateManager::setFlag(uint32 f){
if((flags.load()&f)!=f){
flags|=f;
return true;
}
return false;
}

bool StateManager::removeFlag(uint32 f){
if((flags.load()&f)==f){
flags^=f;
return true;
}
return false;
}

bool StateManager::flag_contains(uint32 f)const{
return (flags.load()&f)==f;
}

uint32 StateManager::getFlags()const{
return flags.load();
}

void StateManager::replaceFlags(uint32 f){
flags.store(f);
}

void StateManager::waitForState(STATEMANAGERCALLBACK hcall){
_GASSERT(hcall!=NULL);
while(hcall(currentState.load())){
this_thread::yield();
}
}

void StateManager::waitForStage(STATEMANAGERCALLBACK hcall){
_GASSERT(hcall!=NULL);
while(hcall(stage.load())){
this_thread::yield();
}
}

}

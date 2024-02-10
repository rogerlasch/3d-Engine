


#ifndef STATEMANAGER_H
#define STATEMANAGER_H

namespace gpp{

typedef std::function<bool(uint32)> STATEMANAGERCALLBACK;
class StateManager{
private:
std::atomic<uint32> lastState;
std::atomic<uint32> currentState;
std::atomic<uint32> lastStage;
std::atomic<uint32> stage;
std::atomic<int64> lastTime;
std::atomic<int64> htime;
std::atomic<uint32> flags;
public:
StateManager(uint32 hs=0);
StateManager(const StateManager& st);
StateManager& operator=(const StateManager& st);
virtual ~StateManager();
void setStage(uint32 s);
void setState(uint32 s);
uint32 getStage()const;
uint32 getLastStage()const;
uint32 getState()const;
uint32 getLastState()const;
int64 getTime()const;
int64 getLastTime()const;
bool setFlag(uint32 f);
bool removeFlag(uint32 f);
bool flag_contains(uint32 f)const;
uint32 getFlags()const;
void replaceFlags(uint32 f);
void waitForState(STATEMANAGERCALLBACK hcall);
void waitForStage(STATEMANAGERCALLBACK hcall);
};
}
#endif

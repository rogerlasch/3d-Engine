


#ifndef WORLDCALLBACKS_H
#define WORLDCALLBACKS_H

namespace gpp{

class WorldCallbacks {
public:
iBroadPhase* bPhase;
iNarrowPhase* nPhase;
iCollisionSolver* iSolver;
iMotion* hMotion;
iFilter* filter;
WorldCallbacks();
WorldCallbacks(const WorldCallbacks& wk)=delete;
WorldCallbacks& operator=(const WorldCallbacks& wk)=delete;
virtual ~WorldCallbacks();
bool isValid()const;
void cleanup();
void createDefault();
void copy_from(WorldCallbacks* wk);
};
}

#endif

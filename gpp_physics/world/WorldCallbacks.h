


#ifndef WORLDCALLBACKS_H
#define WORLDCALLBACKS_H

namespace gpp{

typedef std::function<void(shared_collisionrow&)> COLLISIONCALLBACK;
class WorldCallbacks {
public:
std::shared_ptr<iBroadPhase> bPhase;
std::shared_ptr<iNarrowPhase> nPhase;
std::shared_ptr<iCollisionSolver> iSolver;
std::shared_ptr<iMotion> hMotion;
std::shared_ptr<iFilter> filter;
COLLISIONCALLBACK hcall;
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

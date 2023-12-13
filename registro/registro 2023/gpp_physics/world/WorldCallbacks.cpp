

#include"../gpp_physics.h"
#include"WorldCallbacks.h"

using namespace std;

namespace gpp
{
WorldCallbacks::WorldCallbacks()
{
bPhase=NULL;
nPhase=NULL;
iSolver=NULL;
hMotion=NULL;
filter=NULL;
hcall=NULL;
}

WorldCallbacks::~WorldCallbacks() {
this->cleanup();
}

bool WorldCallbacks::isValid()const
{
if((bPhase==NULL)||(nPhase==NULL)) return false;
if((hMotion==NULL)||(iSolver==NULL)) return false;
if(filter==NULL)return false;
return true;
}

void WorldCallbacks::cleanup() {
bPhase=NULL;
nPhase=NULL;
iSolver=NULL;
hMotion=NULL;
hcall=NULL;
}

void WorldCallbacks::createDefault()
{
this->cleanup();
bPhase=make_shared< BroadPhase>();
nPhase=make_shared< NarrowPhase>();
iSolver=make_shared< CollisionSolver>();
hMotion=make_shared< DefaultMotion>();
filter=make_shared< DefaultFilter>();
hcall=NULL;
}

void WorldCallbacks::copy_from(WorldCallbacks* wk)
{
this->cleanup();
this->bPhase=wk->bPhase;
this->nPhase=wk->nPhase;
this->iSolver=wk->iSolver;
this->hMotion=wk->hMotion;
this->filter=wk->filter;
this->hcall=wk->hcall;
}
}

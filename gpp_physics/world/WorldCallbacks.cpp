

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
if(bPhase!=NULL) delete bPhase;
if(nPhase!=NULL)delete nPhase;
if(iSolver!=NULL) delete iSolver;
if(hMotion!=NULL) delete hMotion;
if(filter!=NULL) delete filter;
bPhase=NULL;
nPhase=NULL;
iSolver=NULL;
hMotion=NULL;
}

void WorldCallbacks::createDefault()
{
this->cleanup();
bPhase=new BroadPhase();
nPhase=new NarrowPhase();
iSolver=new CollisionSolver();
hMotion=new DefaultMotion();
filter=new DefaultFilter();
}

void WorldCallbacks::copy_from(WorldCallbacks* wk)
{
this->cleanup();
this->bPhase=wk->bPhase;
this->nPhase=wk->nPhase;
this->iSolver=wk->iSolver;
this->hMotion=wk->hMotion;
this->filter=wk->filter;
}
}

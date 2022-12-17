

#include"gpp_world.h"
#include"WordEvent.h"

using namespace std;

namespace gpp
{
WordEvent::WordEvent()
{
type=WE_DEFAULT;
v1=NULL;
v2=NULL;
}

WordEvent::~WordEvent()
{
type=WE_DEFAULT;
v1=NULL;
v2=NULL;
}
}

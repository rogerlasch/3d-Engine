
#include "EventFrame.h"
#include <iostream>

using namespace std;

namespace gpp {

// Construtor padrão
EventFrame::EventFrame(){
id=0;
counter=0;
frequence=0;
userdata=NULL;
v1=NULL;
v2=NULL;
hcall=NULL;
oneFire=false;
}

void EventFrame::cleanup(){
id=0;
counter=0;
frequence=0;
userdata=NULL;
v1=NULL;
v2=NULL;
hcall=NULL;
oneFire=false;
}

EventFrame* EventFrame::clone(){
EventFrame* sf=new EventFrame();
sf->id=this->id;
sf->counter=this->counter;
sf->frequence=this->frequence;
sf->hcall=this->hcall;
sf->userdata=this->userdata;
sf->v1=this->v1;
sf->v2=this->v2;
sf->oneFire=this->oneFire;
return sf;
}

}

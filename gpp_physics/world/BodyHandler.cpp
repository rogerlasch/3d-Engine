
#include<sstream>
#include"../math/math.h"
#include"../geometry/geometry.h"
#include"WorldProperties.h"
#include"BodyHandler.h"
#include"RigidBody.h"
#include"DefaultBodyHandler.h"

using namespace std;

namespace gpp{

BodyHandler::BodyHandler(){
this->cleanup();
}

BodyHandler::~BodyHandler(){
this->cleanup();
}

string BodyHandler::toString()const{
stringstream ss;
ss<<"Prestep? "<<boolalpha<<(preStep!=NULL)<<endl;
ss<<"Step? "<<boolalpha<<(step!=NULL)<<endl;
ss<<"OnLoop? "<<boolalpha<<(onLoop!=NULL)<<endl;
ss<<"BeginContact? "<<boolalpha<<(beginContact!=NULL)<<endl;
ss<<"UpdateContact? "<<boolalpha<<(updateContact!=NULL)<<endl;
ss<<"EndContact? "<<boolalpha<<(endContact!=NULL)<<endl;
return ss.str();
}

void BodyHandler::cleanup(){
preStep=NULL;
step=NULL;
onLoop=NULL;
beginContact=NULL;
updateContact=NULL;
endContact=NULL;
}

void BodyHandler::toDefault(){
cleanup();
preStep = defaultpreStep;
step=defaultStep;
}

void BodyHandler::copyFrom(BodyHandler* handler){
this->preStep=handler->preStep;
this->step=handler->step;
this->onLoop=handler->onLoop;
this->beginContact=handler->beginContact;
this->updateContact=handler->updateContact;
this->endContact=handler->endContact;

}
}

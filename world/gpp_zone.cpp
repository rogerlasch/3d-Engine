

#include"geometry.h"
#include"gpp_world.h"

using namespace std;

namespace gpp
{
gpp_zone::gpp_zone()
{
parent=NULL;
childs.clear();
}

gpp_zone::~gpp_zone()
{
if(childs.size()>0)
{
for(auto it=childs.begin(); it!=childs.end(); ++it)
{
delete (*it);
(*it)=NULL;
}
}
childs.clear();
parent=NULL;
}

void gpp_zone::setFloor(const gpp_floor& f)
{
this->floor=f;
}

gpp_floor gpp_zone::getFloor()const
{
return this->floor;
}

void gpp_zone::addChild(gpp_zone* gz)
{
profiler_snap();
BinarySearcher<gpp_zone> bs;
if(bs.findUnique(childs, gz->index, NULL))
{
_FLOG("Achou {}", gz->index.toString());
return;
}
InserterSorted<gpp_zone> is;
gz->parent=this;
is(childs, gz);
}

bool gpp_zone::removeChild(const gpp_index& id)
{
uint32 i=0;
if(this->getChild(id, &i))
{
return false;
}
delete childs[i];
childs.erase(childs.begin()+i);
return true;
}

gpp_zone* gpp_zone::getChild(const gpp_index& id, uint32* index)
{
BinarySearcher<gpp_zone> bs;
uint32 i=0;
if(!bs.findUnique(childs, id, &i))
{
return NULL;
}
if(index!=NULL)
{
*index=i;
}
return childs[i];
}

string gpp_zone::toString()const
{
stringstream ss;
ss<<"Nome: "<<this->getName()<<endl;
ss<<"Id: "<<index.toString()<<endl;
ss<<body->getBoundingBox().toString();
ss<<"Pai: "<<((parent==NULL) ? "NULL" : parent->getName())<<endl;
for(auto& it : childs)
{
ss<<it->toString()<<endl;
}
return ss.str();
}

void gpp_zone::onStep(float dt, vector3d* onchange)
{
RigidBody::onStep(dt, onchange);
if(onchange!=NULL)
{
translate(*onchange);
}
}

void gpp_zone::translate(const vector3d& v)
{
position+=v;
if(body!=NULL)
{
body->translate(v);
}
for(auto& it : childs)
{
it->translate(v);
}
}

}

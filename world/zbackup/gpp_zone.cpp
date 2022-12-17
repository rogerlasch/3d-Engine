

#include"geometry/geometry.h"
#include"gpp_world.h"

using namespace std;

namespace gpp
{
gpp_zone::gpp_zone():
RigidBody(RB_ZONE)
{
parent=NULL;
setBodyFlag(RB_TRANSPARENT);
childs.clear();
bodies.clear();
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
bodies.clear();
parent=NULL;
}

void gpp_zone::setGround(const gpp_ground& f)
{
this->ground=f;
}

gpp_ground gpp_zone::getGround()const
{
return this->ground;
}

string gpp_zone::toString()const
{
stringstream ss;
ss<<"Nome: "<<this->getName()<<endl;
ss<<"Id: "<<index.toString()<<endl;
ss<<AABB::toString();
ss<<"Pai: "<<((parent==NULL) ? "NULL" : parent->getName())<<endl;
for(auto& it : childs)
{
ss<<it->toString()<<endl;
}
return ss.str();
}

void gpp_zone::translate(const vector3d& v)
{
AABB::translate(v);
for(auto& it : childs)
{
it->translate(v);
}
}

void gpp_zone::setGroundType(uint32 type)
{
gpp_ground_get(type, &ground);
}

gpp_zone* gpp_zone::getParent()const
{
return this->parent;
}

void gpp_zone::setParent(gpp_zone* z)
{
this->parent=z;
}

bool gpp_zone::addChild(gpp_zone* a)
{
if(BinaryUtils::insert(childs, a))
{
a->setParent(this);
return true;
}
return false;
}

bool gpp_zone::removeChild(const gpp_index& id)
{
gpp_zone* a=NULL;
return BinaryUtils::remove(childs, id, &a);
}

gpp_zone* gpp_zone::getChild(const gpp_index& id)
{
uint32 x=0;
if(BinaryUtils::find(childs, id, &x))
{
return childs[x];
}
return NULL;
}

bool gpp_zone::addBody(RigidBody* a)
{
_GASSERT(this->getShape()!=NULL);
for(auto& it : childs)
{
if(it->addBody(a))
{
return true;
}
}
vector3d v=a->getShape()->getCenter();
if(collision3d::isColliding(this->getShape(), v))
{
a->setZone(this);
BinaryUtils::insert(bodies, a);
return true;
}
return false;
}

bool gpp_zone::removeBody(const gpp_index& id)
{
RigidBody* a=NULL;
return BinaryUtils::remove(bodies, id, &a);
}

}



#include<algorithm>
#include"gpp_world.h"

using namespace std;

namespace gpp
{
gpp_world::gpp_world()
{
}

gpp_world::~gpp_world()
{
profiler_snap();
for_each(bodies.begin(), bodies.end(), BodyDeletor());
}

void gpp_world::setGroundLevel(float lv)
{
this->ground_level=lv;
}

float gpp_world::getGroundLevel()const
{
return this->ground_level;
}

void gpp_world::setGravity(const vector3d& gravity)
{
this->gravity=gravity;
}

vector3d gpp_world::getGravity()const
{
return this->gravity;
}

void gpp_world::setLowerBound(const vector3d& lowerbound)
{
this->lower_bound=lowerbound;
}

vector3d gpp_world::getLowerBound()const
{
return this->lower_bound;
}

void gpp_world::setHigherBound(const vector3d& higherbound)
{
this->higher_bound=higherbound;
}

vector3d gpp_world::getHigherBound()const
{
return this->higher_bound;
}

void gpp_world::setRoot(gpp_zone* root)
{
if(this->root!=NULL)
{
delete this->root;
}
this->root=root;
}

gpp_zone* gpp_world::getRoot()const
{
return this->root;
}

uint32 gpp_world::where_is_point(const vector3d& v)const
{
if((v.z<lower_bound.z)||(v.z>higher_bound.z))
{
return WHERE_UNKNOWN;
}
if(v.z>getGroundLevel())
{
return WHERE_AIR;
}
if(v.z==getGroundLevel())
{
return WHERE_GROUND;
}
return WHERE_UNDERGROUND;
}

bool gpp_world::point_in_world(const vector3d& v)const
{
if((v.x<lower_bound.x)||(v.x>higher_bound.x)||(v.y<lower_bound.y)||(v.y>higher_bound.y)||(v.z<lower_bound.z)||(v.z>higher_bound.z))
{
return false;
}
return true;
}

bool gpp_world::addBody(uint32 vnum, uint32 subvnum)
{
profiler_snap();
BinarySearcher<RigidBody> sh;
InserterSorted<RigidBody> is;
if(sh.findUnique(bodies, {vnum, subvnum}, NULL))
{
return false;
}
RigidBody* b=new RigidBody();
b->setIndex({vnum, subvnum});
b->setMass(random_float(1, 1000));
b->setPosition({random_float(0, higher_bound.x), random_float(0, higher_bound.y)});
b->setForse(gravity);
is(bodies, b);
return true;
}

void gpp_world::add_bodies(uint32 qtd, uint32 subqtd)
{
profiler_snap();
for(uint32 i=0; i<qtd; i++)
{
for(uint32 i1=0; i1<subqtd; i1++)
{
addBody((i+1), (i1+1));
}
}
_FLOG("Total de corpos: {}", bodies.size());
}

vector<RigidBody*>& gpp_world::getBodies()
{
return bodies;
}

void gpp_world::update(float dt)
{
profiler_snap();
for(auto& it : bodies)
{
it->onStep(dt);
}
}

string gpp_world::toString()const
{
stringstream ss;
ss<<fixed;
ss.precision(2);
ss<<"Estatísticas do mundo:"<<endl;
ss<<"Ground_level: "<<ground_level<<endl;
ss<<"Gravidade: "<<gravity<<endl;
ss<<"LowerBound: "<<lower_bound<<endl;
ss<<"HigherBound: "<<higher_bound<<endl;
ss<<"Total de corpos no mundo: "<<bodies.size()<<endl;
return ss.str();
}
}


#include"MurmurHash3.h"
#include<sstream>
#include"collision.h"
#include"CollisionInfo.h"

using namespace std;

namespace gpp
{

CollisionInfo::CollisionInfo()
{
id=0;
type=COL_NONE;
frames=0;
point.zero();
lastPoint.zero();
normal.zero();
lastNormal.zero();
depth=0.0f;
lastDepth=0.0f;
r1=NULL;
r2=NULL;
}

CollisionInfo::CollisionInfo(const CollisionInfo& c)
{
*this=c;
}

CollisionInfo& CollisionInfo::operator=(const CollisionInfo& c)
{
this->id=c.id;
this->type=c.type;
this->frames=c.frames;
this->point=c.point;
this->lastPoint=c.lastPoint;
this->normal=c.normal;
this->lastNormal=c.lastNormal;
this->depth=c.depth;
this->lastDepth=c.lastDepth;
this->r1=c.r1;
this->r2=c.r2;
return *this;
}

bool CollisionInfo::operator<(const CollisionInfo& info)const{
uint64 a1=(uint64)r1+(uint64)r2;
uint64 a2=(uint64)info.r1+(uint64)info.r2;
return a1 < a2;
}

uint64 CollisionInfo::getId()const{
return this->id;
}

void CollisionInfo::reset()
{
id=0;
type=COL_NONE;
frames=0;
point.zero();
lastPoint.zero();
normal.zero();
lastNormal.zero();
depth=0.0f;
lastDepth=0.0f;
r1=NULL;
r2=NULL;
}

string CollisionInfo::toString()const
{
stringstream ss;
ss<<fixed;
ss.precision(2);
ss<<"Id="<<id<<", Type="<<type<<endl;
ss<<"lastPoint="<<lastPoint<<", Point="<<point<<endl;
ss<<"lastNormal="<<lastNormal<<", Normal="<<normal<<endl;
ss<<"lastDepth="<<lastDepth<<", Depth="<<depth<<endl;
return ss.str();
}

void CollisionInfo::backup(){
this->lastPoint=point;
this->lastNormal=normal;
this->lastDepth=depth;
}

void CollisionInfo::update(CollisionInfo* info){
this->backup();
this->point=info->point;
this->normal=info->normal;
this->depth=info->depth;
}

ostream& operator<<(ostream& os, const CollisionInfo& cd)
{
os<<"Id: "<<cd.id<<endl;
os<<"ponto: "<<cd.point<<endl;
os<<"normal: "<<cd.normal<<endl;
os<<"depth: "<<cd.depth<<endl;
return os;
}

uint64 getCollisionId(uint32 id1, uint32 id2){
profiler_snap();
const static uint32 seed=0xFFFFFFFB;
uint32 arr[2]={id1, id2};
uint64 hash=0;
MurmurHash3_x64_128 (arr, sizeof(arr), seed, &hash);
return hash;
}
}

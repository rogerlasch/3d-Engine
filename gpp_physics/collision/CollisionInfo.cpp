
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
lastType=COL_NONE;
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
this->lastType=c.lastType;
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
uint64 a1=((id==0) ? getCollisionId((uint64)r1, (uint64)r2) : id);
uint64 a2=((info.id==0) ? getCollisionId((uint64)info.r1, (uint64)info.r2) : info.id);
return a1 < a2;
}

uint64 CollisionInfo::getId()const{
return this->id;
}

uint64 CollisionInfo::generateId(){
this->id=getCollisionId((uint64)r1, (uint64)r2);
return id;
}

void CollisionInfo::reset()
{
id=0;
type=COL_NONE;
lastType=COL_NONE;
point.zero();
lastPoint.zero();
normal.zero();
lastNormal.zero();
depth=0.0f;
lastDepth=0.0f;
r1=NULL;
r2=NULL;
}

std::string CollisionInfo::toString() const {
    std::stringstream ss;
    ss << std::fixed;
    ss.precision(2);
    ss << "CollisionInfo:" << std::endl;
    ss << "Id=" << id << ", Type=" << type << std::endl;
    ss << "Point=" << point << ", lastPoint=" << lastPoint << std::endl;
    ss << "Normal=" << normal << ", lastNormal=" << lastNormal << std::endl;
    ss << "Depth=" << depth << ", lastDepth=" << lastDepth << std::endl;
    return ss.str();
}

void CollisionInfo::backup(){
this->lastType=type;
this->lastPoint=point;
this->lastNormal=normal;
this->lastDepth=depth;
}

void CollisionInfo::update(CollisionInfo* info){
this->backup();
this->point=info->point;
this->normal=info->normal;
this->depth=info->depth;
if((type==COL_BEGIN)&&(info->type==COL_BEGIN)){
this->type=COL_CONTINUOUS;
}
}

ostream& operator<<(ostream& os, const CollisionInfo& cd)
{
os<<"Id: "<<cd.id<<endl;
os<<"ponto: "<<cd.point<<endl;
os<<"normal: "<<cd.normal<<endl;
os<<"depth: "<<cd.depth<<endl;
return os;
}

uint64 getCollisionId(uint64 id1, uint64 id2){
const static uint32 seed=0xFFFFFFFB;
uint64 arr[2]={id1, id2};
uint64 hash=0;
MurmurHash3_x64_128 (arr, sizeof(arr), seed, &hash);
return hash;
}
}

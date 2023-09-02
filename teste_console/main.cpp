#include <iostream>
#include<stack>
#include<unordered_set>
#include <deque>
#include <gpp/gpp.h>
#include <gpp_physics/gpp_physics.h>
#include <cmath>
#include"g_start.h"

using namespace gpp;
using namespace std;

vector<shared_ptr<iRigidBody>> hbodies;
CollisionRow rows;
RigidBody* createRandomBody();
void fillRows(uint32 n);
int main()
{
G_STARTER hstart;
fillRows(5000);
CollisionManager dg;
for(auto& it : rows){
dg.insert(it);
}
_GINFO("{}", dg.toString());
uint32 x=dg.removeByHashs({rows[0]->id, rows[2]->id});
_GINFO("Removido? {}", x);
_GINFO("{}", dg.toString());
x=dg.removeByIndexs({rows[3]->r1->index, rows[30]->r2->index});
_GINFO("Removido? {}", x);
_GINFO("{}", dg.toString());
for(auto& it : rows){
_GINFO("{}", it->id);
}
return 0;
}

RigidBody* createRandomBody()
{
static uint32 x=0;
x++;
shared_ptr<RigidBody> rb=make_shared<RigidBody>();
rb->setIndex({x, x+1});
sphere3d* sp=new sphere3d();
sp->center.x=random_float(50, 99950);
sp->center.y=random_float(50, 99950);
sp->center.z=random_float(50, 99950);
sp->radius=random_float(2.5, 45.0f);
rb->setGeometricShape(sp);
hbodies.push_back(rb);
return rb.get();
}

void fillRows(uint32 n)
{
unordered_set<uint64> hst;
for(uint32 i=0; i<n; i++)
{
shared_collisioninfo info=make_shared<CollisionInfo>();
if(hbodies.size()<100000){
info->r1=createRandomBody();
info->r2=createRandomBody();
}
else{
uint32 v1=0, v2=0;
while(v1==v2){
v1=random_uint32(0, hbodies.size()-1);
v2=random_uint32(0, hbodies.size()-1);
}
info->r1=hbodies[v1].get();
info->r2=hbodies[v2].get();
}
info->id=get_hash_from_index(info->r1->index, info->r2->index);
if(hst.find(info->id)==hst.end()){
rows.push_back(info);
hst.insert(info->id);
}
}
}

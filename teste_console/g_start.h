


#ifndef G_START_H
#define G_START_H

#include<sstream>
#define F_NAME "main.txt"

namespace gpp{
/*
std::vector<std::shared_ptr<iRigidBody>> hbodies;

std::shared_ptr<gpp_world> game;
RigidBody* createRandomBody();
RigidBody* createSphere(const vector3d& center, float radius);
RigidBody* createBox(const vector3d& min, const vector3d& mes);
RigidBody* createCapsule(const vector3d& min, const vector3d& max, float radius);
void displayInfos(RigidBody* rb);
std::string getDirectionDescription(const vector3d& dir) ;
*/
struct G_STARTER{
G_STARTER(){
setlocale(LC_ALL, "Portuguese");
profiler_start();
log_create(F_NAME, LG_ALL);
log_set_default(F_NAME);
std::cout<<std::fixed;
std::cout.precision(2);
}
~G_STARTER(){
profiler_dump("main_profiler.txt");
}
};

/*
RigidBody* createRandomBody()
{
static uint32 x=0;
x++;
std::shared_ptr<RigidBody> rb=std::make_shared<RigidBody>();
rb->setIndex({x, x+1});
rb->mass=random_float(1.0f, 330.0f);
rb->restitution=random_float(0.01f, 1.0f);
hbodies.push_back(rb);
return rb.get();
}

RigidBody* createSphere(const vector3d& center, float radius){
RigidBody* rb=createRandomBody();
sphere3d* sp=new sphere3d();
sp->center=center;
sp->radius=radius;
rb->setGeometricShape(sp);
return rb;
}

RigidBody* createBox(const vector3d& min, const vector3d& mes){
RigidBody* rb=createRandomBody();
box3d* b=new box3d(min, mes);
rb->setGeometricShape(b);
return rb;
}

RigidBody* createCapsule(const vector3d& min, const vector3d& max, float radius){
RigidBody* rb=createRandomBody();
capsule3d* b=new capsule3d(min, max, radius);
rb->setGeometricShape(b);
return rb;
}

void displayInfos(RigidBody* rb) {
vector3d vdir=rb->position-rb->getGeometricShape()->getLastPosition();
vdir.normalize();
std::stringstream ss;
ss<<std::fixed;
ss.precision(2);
ss<<"T: "<<game->info->currenttime<<"S, Dir: "<<getDirectionDescription(vdir)<<::std::endl;
ss<<"Pos: "<<rb->position<<", Vel: "<<rb->velocity.length()<<"M/S"<<::std::endl;
_GINFO("{}", ss.str());
}

std::string getDirectionDescription(const vector3d& dir) {
std::stringstream ss;
if(dir.x<0) ss<<"o";
else if(dir.x>0) ss<<"l";
if(dir.y<0) ss<<"s";
else if(dir.y>0) ss<<"n";
if(dir.z<0) ss<<"b";
else if(dir.z>0) ss<<"c";
return ss.str();
}
*/
}
#endif

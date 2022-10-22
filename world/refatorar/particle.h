

#ifndef PARTICLE_H
#define PARTICLE_H

namespace gpp
{

class particle
{
public:
vector3d position;
vector3d velocity;
float mass;
particle(const vector3d& pos, const vector3d& v, float mass)
{
this->position=pos;
this->velocity=v;
this->mass=mass;
}
particle(const particle& p)
{
*this=p;
}
particle& operator=(const particle& p)
{
this->position=p.position;
this->velocity=p.velocity;
this->mass=p.mass;
return *this;
}
void onStep(const vector3d& forse, float ms=0.05)
{
vector3d v((this->mass*forse.x), (this->mass*forse.y), (this->mass*forse.z));
vector3d acceleration((v.x/this->mass), (v.y/this->mass), (v.z/this->mass));
velocity.x+=(acceleration.x*ms);
velocity.y+=(acceleration.y*ms);
velocity.z+=(acceleration.z*ms);
position.x+=(velocity.x*ms);
position.y+=(velocity.y*ms);
position.z+=(velocity.z*ms);
}
};
}
#endif

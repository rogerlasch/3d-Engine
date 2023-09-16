#include <iostream>
#include<sstream>
#include <gpp/gpp.h>
#include <gpp_physics/gpp_physics.h>
#include"g_start.h"
#include"testes.h"

using namespace gpp;
using namespace std;

int main()
{
G_STARTER hstart;
/*
WorldInfo winfo;
winfo.flags=GWF_GRAVITY;
winfo.min={0,0,-10000.00f};
winfo.max={1000000, 1000000, 1000000};
winfo.gravity={9.81f, 0.0f, -9.81f};
game=make_shared<gpp_world>(&winfo);
RigidBody* ground=createBox(winfo.min, {winfo.max.x, winfo.max.y, fabs(winfo.min.z)});
ground->setName("Chão");
ground->mass=0.0f;
game->addBody(ground);

RigidBody* obs=createCapsule({50,0,-15}, {50,100,-15}, 25.0f);
obs->setName("Elevado");
obs->mass=0.0f;
game->addBody(obs);

RigidBody* rb=createSphere({5.0f, 10.0f, 2.5f}, 2.5f);
rb->setName("Esfera dinâmica");
game->addBody(rb);
rb->mass=25.0f;
rb->restitution=0.01f;
_GINFO("{}", game->toString());
for(uint32 i=0; i<2500; i++)
{
game->update(0.2f);
displayInfos(rb);
}
_GINFO("Fim da simulação...\n{}", game->toString());
*/
vector3d v={10,0,-1.96};
vector3d n={-0.66,0,0.53};
float depth=1.25;
vector3d vw=n*depth;
vw.normalize();
cout<<vw<<endl;
return 0;
}

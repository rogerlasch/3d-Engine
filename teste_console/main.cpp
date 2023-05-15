#include <iostream>
#include <gpp/gpp.h>
#include <gpp_physics/gpp_physics.h>
#include <cmath>

using namespace gpp;
using namespace std;

#define LOG_FILENAME "main.txt"

class player
{
public:
vector3d position;
vector3d dir;
quaternion orientation;
player()
{
position={0,0,0};
dir={0,1,0};
orientation=quaternion_from_euler_angles(0,0,0);
}
void step(float dist)
{
vector3d movement=(quaternion_vector_rotate(orientation, dir)*0.2f)*dist;

position+=movement;
}
};

int main()
{
setlocale(LC_ALL, "Portuguese");
log_create(LOG_FILENAME, LG_ALL);
log_set_default(LOG_FILENAME);
shared_ptr<player> ch=make_shared<player>();
ch->orientation=quaternion_from_euler_angles(0,0,315);
for(int i=0; i<10; i++)
{
_GINFO("{}", ch->position.toString());
ch->step(1.5f);
}
return 0;
}

#include <iostream>
#include <gpp/gpp.h>
#include <gpp_physics/gpp_physics.h>
#include <cmath>

using namespace gpp;
using namespace std;

int main()
{
box3d b({0,0,0}, {20,20,20});
quaternion original=quaternion_from_euler_angles(0,0,-90);
quaternion q;
q.setIdentity();
for(int i=0; i<8; i++)
{
q=q*original;
b.Rotate(q);
cout<<b.min<<(b.min+b.measures)<<endl;
}
    return 0;
}

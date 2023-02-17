#include <iostream>
#include <gpp/gpp.h>
#include <gpp_physics/gpp_physics.h>
#include <cmath>
#include"Simplex3d.h"

using namespace gpp;
using namespace std;

typedef vector<vector3d> polyedron;
ostream& operator<<(ostream& os, const polyedron& poly);
int main()
{
setlocale(LC_ALL, "Portuguese");
//polyedron p1={{0,0,0}, {0,10,0}, {10,10,0}, {10,0,0}, {0,0,10}, {0,10,10}, {10,10,10}, {10,0,10}};
//polyedron p2={{10,0,0}, {10,10,0}, {20,10,0}, {20,0,0}, {10,0,10}, {10,10,10}, {20,10,10}, {20,0,10}};
return 0;
}

ostream& operator<<(ostream& os, const polyedron& poly)
{
for(auto& it : poly)
{
os<<it<<endl;
}
return os;
}

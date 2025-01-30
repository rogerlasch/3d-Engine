
#define GPP_DOUBLE_PRECISION
#include <iostream>
#include<random>
#include <iomanip>
#include <gpp/gpp.h>
#include <gpp_physics/gpp_physics.h>
#include<gpp_physics/math/math.h>
#include"g_start.h"

using namespace gpp;
using namespace std;
#include"bvh.h"

int main() {
G_STARTER gst;

bvh b1({0,0,0}, {1000, 1000, 1000});
bvh b2({900, 900, 900}, {1000, 1000, 1000});

b1.insert(&b2);

vector3d origin={0,0,0};
vector3d dir={1,1,1};
vector<RayInfo> infos;
b1.rayCast(origin, dir, infos);
logger::info("Total de colisões: {}", infos.size());
for(auto& it : infos){
logger::info("{}\n", it.toString());
}

    return 0;
}

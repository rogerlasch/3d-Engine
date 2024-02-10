#include <iostream>
#include<sstream>
#include <gpp/gpp.h>
#include <gpp_physics/gpp_physics.h>
#include"g_start.h"

using namespace gpp;
using namespace std;
#include"LinearOctree.h"

class Cylinder{
public:
vector<vector3d> topCap;
vector<vector3d> botomCap;
float radius;
Cylinder(){
topCap.clear();
botomCap.clear();
radius=1.5f;
}
void generate(uint32 numSides, float height, float radius){
this->radius=radius;
topCap.clear();
botomCap.clear();
    // Calcular o ângulo entre cada lado do cilindro
    float angleIncrement = 2 * GPP_PI / numSides;
    // Gerar vértices em torno da base do cilindro
    for (int i = 0; i < numSides; ++i) {
        float angle = i * angleIncrement;
        vector3d vertex;
        vertex.x = radius * cos(angle);
        vertex.y = radius * sin(angle);
        vertex.z = 0.0f;  // Altura da base
botomCap.push_back(vertex);
    }
    // Gerar vértices em torno do topo do cilindro
    for (int i = 0; i < numSides; ++i) {
        float angle = i * angleIncrement;
        vector3d vertex;
        vertex.x = radius * cos(angle);
        vertex.y = radius * sin(angle);
        vertex.z = height;  // Altura do topo
topCap.push_back(vertex);
    }
    // Adicionar vértices para as bases
    vector3d baseCenter = {0.0f, 0.0f, 0.0f};
    vector3d topCenter = {0.0f, 0.0f, height};
botomCap.push_back(baseCenter);
topCap.push_back(topCenter);
}
string toString()const{
stringstream ss;
ss<<fixed;
ss.precision(2);
ss<<"Radius: "<<radius<<endl;
for(auto& it : botomCap){
ss<<it<<endl;
}
for(auto& it : topCap){
ss<<it<<endl;
}
return ss.str();
}
bool isColliding(const vector3d& v){
uint32 size=topCap.size();
for(uint32 i=0; i<size; i++){
uint32 j=(i+1)%size;
uint32 k=(i+2)%size;
vector3d n=vector3d::crossProduct(botomCap[j]-botomCap[i], botomCap[k]-botomCap[i]);
n.normalize();
_GINFO("Dot={}", n*v);
}
vector3d n=topCap[size-1]-botomCap[size-1];
n.normalize();
_GINFO("\n{}", n);
_GINFO("tsk {}", (v*n));
return false;
}
};

int main(){
G_STARTER gst;
ln::OctreeNode2* node=new ln::OctreeNode2();
node->center={100,100,100};
node->radius=100.0f;
ln::LinearOctree oct;
oct.appendNode(node);
oct.build(node);
//_GINFO("{}", oct.toString());
oct.traverse(node->id);
return 0;
}

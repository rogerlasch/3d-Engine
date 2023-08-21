#include <iostream>
#include<stack>
#include <deque>
#include <gpp/gpp.h>
#include <gpp_physics/gpp_physics.h>
#include <cmath>
#include"g_start.h"

using namespace gpp;
using namespace std;

vector<shared_ptr<iRigidBody>> hbodies;
RigidBody* createRandomBody();
bool rayPoint(const vector3d& origin, const vector3d& dir, const vector3d& point);
bool sphereBox(sphere3d* s, box3d* b, CollisionInfo* info);
bool raySphere(const vector3d& origin, const vector3d& dir, sphere3d* s, vector3d& q);
bool rayBox(const vector3d& origin, const vector3d& dir, box3d* b, vector3d& q, bool calculateEntry = true);

int main()
{
G_STARTER hstart;
box3d b({0,0,-10}, {100,100,10});
sphere3d s({10,10,10}, 2.5f);
for(uint32 i=0; i<20; i++)
{
s.translate({0,0,-2});
_GINFO("{:.2f}", s.center.z);
CollisionInfo info;
if(sphereBox(&s, &b, &info)) {
_GINFO("Colisão: {}", info.toString());
if(info.depth>0)
{
s.translate(info.normal*info.depth);
}
}
}
return 0;
}

void addBodies(octree* oct, uint32 n)
{
for(uint32 i=0; i<n; i++)
{
oct->insert(createRandomBody());
}
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


bool rayPoint(const vector3d& origin, const vector3d& dir, const vector3d& point) {
    // Calcula o vetor que aponta do jogador até o ponto
    vector3d targetDirection = point - origin;

    // Normaliza o vetor de direção do jogador
    vector3d normalizedDir = vector3d::normalize(dir);

    // Normaliza o vetor que aponta do jogador até o ponto
    vector3d normalizedTarget = vector3d::normalize(targetDirection);

    // Calcula o produto escalar entre os vetores normalizados
    float dotProductValue = vector3d::dotProduct(normalizedDir, normalizedTarget);

    // Verifica se o produto escalar é maior ou igual a 0
    // Isso indica que o vetor dir aponta na mesma direção que o vetor jogador -> ponto
_GINFO("{:.2f}", dotProductValue );
    return dotProductValue > 0.0f;
}

bool sphereBox(sphere3d* s, box3d* b, CollisionInfo* info)
{
profiler_snap();
//Recuperar o ponto mais próximo da caixa em relação ao centro da esfera...
vector3d closestPoint=get_closest_point_from_aabb(s->center, b->min, b->max);
//Calcular a distância ao quadrado do ponto e o centro da esfera
//Depois, verificar se a distância é menor ou igual ao raio ao quadrado da esfera...
vector3d vdir=s->center-closestPoint;
float sqDist=vdir*vdir;
float sqRadius=s->radius*s->radius;
//Se a distância for maior, não à colisão...
if(sqDist>sqRadius) return false;
//Caso o centro da esfera esteja dentro da caixa...
//O vetor normal da colisão será nulo, e precisamos tratar isso...
//Aqui, calcularemos a direção da tragetória e depois lançaremos um raio na direção oposta para encontrar o ponto de saída da caixa.
//Por fim, definiremos o vetor normal apontando na direção em que o raio foi disparado.
vector3d normal;
float depth=0.0f;
vector3d contactPoint;
//Verifica se o centro da esfera está dentro dos limites da caixa...
if(point_in_aabb(s->center, b->min, b->max))
{
//Primeiro, calcular o vetor de direção desde a última posição conhecida...
vector3d lastPosition=s->getLastPosition();
vector3d from=s->center-lastPosition;
from.normalize();
vector3d fromInverse=vector3d::inverse(from);
vector3d pt;
//Vamos lançar um raio na direção oposta da tragetória para determinarmos o ponto de saída.
rayBox(s->center, fromInverse, b, pt, false);
normal=fromInverse;
contactPoint=pt;
//Para a profundidade de penetração primeiro devemos calcular o ponto máximo de penetração...
//E depois calcular a distância entre ele e o ponto de contato...
vector3d maxPenetration=s->center+(s->radius*from);
depth=fabs((from*(maxPenetration-contactPoint)));
}
else//O centro da esfera está fora da caixa, por tanto, apenas calcule as informações...
{
contactPoint=closestPoint;
normal=(s->center-closestPoint).normalize();
vector3d maxPenetration=(s->center+(vector3d::inverse(normal)*s->radius));
depth=normal*(maxPenetration-contactPoint);
}
info->normal=normal;
info->point=contactPoint;
info->depth=fabs(depth);
return true;
}


//Código retirado do realtime collision detection...
bool raySphere(const vector3d& origin, const vector3d& dir, sphere3d* s, vector3d& q)
{
          vector3d m = origin - s->center;
          float b = vector3d::dotProduct(m, dir);
          float c = vector3d::dotProduct(m, m) - s->radius * s->radius;
          // Exit if r's origin outside s (c > 0) and r pointing away from s (b > 0)
          if (c > 0.0f && b > 0.0f) return false;
          float discr = b*b - c;
          // A negative discriminant corresponds to ray missing sphere
          if (discr < 0.0f) return false;
          // Ray now found to intersect sphere, compute smallest t value of intersection
float           t = -b - sqrt(discr);
          // If t is negative, ray started inside sphere so clamp t to zero
          if (t < 0.0f) t = 0.0f;
          q = origin + t * dir;
          return true;
}

bool rayBox(const vector3d& origin, const vector3d& dir, box3d* b, vector3d& q, bool calculateEntry
){
    float tmin = 0.0f;
    float tmax = FLT_MAX;

    for (int i = 0; i < 3; i++) {
        if (fabs(dir[i]) < numeric_limits<float>::epsilon()) {
            if (origin[i] < b->min[i] || origin[i] > b->max[i]) return false;
        } else {
            float ood = 1.0f / dir[i];
            float t1 = (b->min[i] - origin[i]) * ood;
            float t2 = (b->max[i] - origin[i]) * ood;
            if (t1 > t2) swap(t1, t2);

            if (calculateEntry) {
                if (t1 > tmin) tmin = t1;
            } else {
                if (t2 < tmax) tmax = t2;
            }

            if (tmin > tmax) return false;
        }
    }

    q = origin + dir * (calculateEntry ? tmin : tmax);
    return true;
}



#include"math.h"

using namespace std;

namespace gpp{

bool computeSphereSphereCollision(const vector3d& c1, float r1, const vector3d& c2, float r2, vector3d& point, vector3d& normal, float& depth)
{
vector3d dt=c1-c2;
float sqDist=(dt*dt);
float rdSun=r1+r2;
//Se dist�ncia^2 maior que soma dos raios^2 n�o a colis�o
if(sqDist>(rdSun*rdSun)){
return false;
}
normal=vector3d::normalize(dt);
depth=fabs(sqrt(sqDist)-rdSun);
vector3d maxPenetration=c1+(vector3d::inverse(normal)*r1);
maxPenetration+=normal*depth;
point=maxPenetration;
normal+=GPP_EPSILON;
    return true;
}

bool computeSphereBoxCollision(const vector3d& lp, const vector3d& closestPoint, const vector3d& center, float radius, const vector3d& min, const vector3d& max, vector3d& point, vector3d& normal, float& depth)
{
//Calcular a dist�ncia ao quadrado do ponto e o centro da esfera
//Depois, verificar se a dist�ncia � menor ou igual ao raio ao quadrado da esfera...
vector3d vdir=center-closestPoint;
float sqDist=vdir*vdir;
float sqRadius=radius*radius;
//Se a dist�ncia for maior, n�o � colis�o...
if(sqDist>sqRadius){
return false;
}
//Caso o centro da esfera esteja dentro da caixa...
//O vetor normal da colis�o ser� nulo, e precisamos tratar isso...
//Aqui, calcularemos a dire��o da traget�ria e depois lan�aremos um raio na dire��o oposta para encontrar o ponto de sa�da da caixa.
//Por fim, definiremos o vetor normal apontando na dire��o em que o raio foi disparado.
//Verifica se o centro da esfera est� dentro dos limites da caixa...
if(point_in_box(center, min, max))
{
//Primeiro, calcular o vetor de dire��o desde a �ltima posi��o conhecida...
vector3d lastPosition=lp;
vector3d from=center-lastPosition;
from.normalize();
vector3d fromInverse=vector3d::inverse(from);
vector3d pt1, pt2;
//Vamos lan�ar um raio na dire��o oposta da traget�ria para determinarmos o ponto de sa�da.
rayBox(center, fromInverse, min, max, pt1, pt2);
normal=fromInverse;
point=pt2;
//Para a profundidade de penetra��o primeiro devemos calcular o ponto m�ximo de penetra��o...
//E depois calcular a dist�ncia entre ele e o ponto de contato...
vector3d maxPenetration=center+(radius*from);
depth=fabs((from*(maxPenetration-point)));
}
else//O centro da esfera est� fora da caixa, por tanto, apenas calcule as informa��es...
{
point=closestPoint;
normal=vector3d::normalize((center-closestPoint));
vector3d maxPenetration=(center+(vector3d::inverse(normal)*radius));
depth=fabs(normal*(maxPenetration-point));
}
normal+=GPP_EPSILON;
return true;
}

bool computeBoxSphereCollision(const vector3d& lp, const vector3d& closestPoint, const vector3d& min, const vector3d& max, const vector3d& center, float radius, vector3d& point, vector3d& normal, float& depth)
{
//Calcular a dist�ncia ao quadrado do ponto e o centro da esfera
//Depois, verificar se a dist�ncia � menor ou igual ao raio ao quadrado da esfera...
vector3d vdir=closestPoint-center;
float sqDist=vdir*vdir;
float sqRadius=radius*radius;
//Se a dist�ncia for maior, n�o � colis�o...
if(sqDist>sqRadius) {
return false;
}
normal=vdir.normalize();
vector3d centerBox=(min+max)*0.5f;
vector3d alph=(centerBox-min);
if(point_in_box(center, min, max)){
normal=(lp-center).normalize();
point=center+(normal*radius);
vector3d maxPenetration=centerBox+(vector3d::inverse(normal)*alph);
maxPenetration=multiVec(normal, maxPenetration);
depth=maxPenetration.length();
}
else{
point=center+(normal*radius);
vector3d maxPenetration=point-closestPoint;
depth=maxPenetration.length();
}
normal+=GPP_EPSILON;
return true;
}
}




#include"../types.h"
#include"vector3d.h"
#include"gjk.h"

using namespace std;
namespace gpp
{
bool GJK::gjk(const vector<vector3d>& p1, const vector<vector3d>& p2, vector<vector3d>& simplex)
{
vector3d center1=findCenterSimplex(p1);
vector3d center2=findCenterSimplex(p2);
if(vector3d::dotProduct(center1, center2)<0)
{
return false;
}
//Calcula o ponto inicial...
vector3d dir=center2-center1;
//Adiciona o primeiro ponto ao simplex...
simplex.push_back(support(p1, dir));
//Inverte a direção...
dir=-dir;
//Adiciona o segundo ponto...
simplex.push_back(support(p2, dir));
dir=center2-center1;
//Loop principal aqui...
uint32 iterations=0;
while(iterations<70)
{
iterations++;
    // encontra o próximo ponto no simplex
vector3d point=support(p2, dir)-support(p1, dir);
    // se o ponto não estiver na direção do vetor entre os dois pontos anteriores, não há colisão
if(vector3d::dotProduct(point, dir)<0)
{
return false;
}
simplex.push_back(point);
if(simplex.size()>4)
{
removeFarthestPoint(simplex, dir);
}
    // verifica se o novo ponto introduzido já está dentro do simplex
    if (isCenterInSimplex(simplex, findCenterSimplex(simplex))) {
return true;
}
}
return false;
}

vector3d GJK::support(const vector<vector3d>& poly, const vector3d& dir)
{
float mag=0.0f;//Guarda a maior magnitude encontrada...
uint32 index=0;//Guarda o índice da maior magnitude...
vector3d dir2=vector3d::normalize(dir);//Normaliza a direção...
for(uint32 i=0; i<poly.size(); i++)
{
float mag2=vector3d::dotProduct(poly[i], dir2);
if(mag2>mag)
{
mag=mag2;
index=i;
}
}
return poly[index];
}

vector3d GJK::findCenterSimplex(const vector<vector3d>& simplex)
{
vector3d center;
for(uint32 i=0; i<simplex.size(); i++)
{
center+=simplex[i];
}
return (center/simplex.size());
}

bool GJK::isCenterInSimplex(const vector<vector3d>& simplex, const vector3d& center)
{
vector3d centerSimplex=findCenterSimplex(simplex);//Busca o centro do simplex...
    for (auto& vertex : simplex) {
        vector3d normal = vector3d::normalize(centerSimplex-vertex);
        vector3d dir = center - vertex;
        if (vector3d::dotProduct(normal, dir) < 0) {
            return false;
        }
    }
    return true;
}

void GJK::removeFarthestPoint(vector<vector3d>& poly, const vector3d& dir)
{
float maxDist=0.0f;
uint32 index=0;
vector3d normal=vector3d::normalize(dir);
for(uint32 i=0; i<poly.size(); i++)
{
float sqDist=vector3d::dotProduct(poly[i], normal);
if(sqDist>maxDist)
{
maxDist=sqDist;
index=i;
}
}
poly.erase(poly.begin()+index);
}
}

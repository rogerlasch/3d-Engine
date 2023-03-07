#include <iostream>
#include <gpp/gpp.h>
#include <gpp_physics/gpp_physics.h>
#include <cmath>

using namespace gpp;
using namespace std;

typedef vector<vector<int32>> matrix;
ostream& operator<<(ostream& os, const matrix& mt);
void rotateMatrix(const quaternion& orientation, matrix& mt);
int main()
{
setlocale(LC_ALL, "Portuguese");
matrix mt={{1,2,3},
{4,5,6},
{7,8,9}};
quaternion orientation=quaternion_from_euler_angles(0,0, -90);
cout<<"Mostrando a matriz original:"<<endl<<mt<<endl;
for(int i=0; i<4; i++)
{
rotateMatrix(orientation, mt);
cout<<"Mostrando a matriz girada em 90 graus no sentido horário..."<<endl<<mt<<endl<<endl;
}
return 0;
}

ostream& operator<<(ostream& os, const matrix& mt)
{
os<<fixed;
for(auto& it : mt)
{
for(auto& it2 : it)
{
os<<it2<<" ";
}
os<<endl;
}
return os;
}

void rotateMatrix(const quaternion& orientation, matrix& mt)
{
matrix mt2;
mt2.resize(mt.size());
for(auto& it : mt2)
{
it.resize(mt.size(), 0);
}
for(int i=0; i<mt.size(); i++)
{
for(int i1=0; i1<mt[i].size(); i1++)
{
vector3d v(i-1, i1-1);
vector3d v2=quaternion_vector_rotate(orientation, v);
int x=round(v2.x)+1;
int y=round(v2.y)+1;
mt2[x][y]=mt[i][i1];
}
}
mt=mt2;
}

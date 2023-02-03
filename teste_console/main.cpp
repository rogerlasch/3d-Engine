#include <iostream>
#include <gpp/gpp.h>
#include <gpp_physics/gpp_physics.h>
#include <cmath>

using namespace gpp;
using namespace std;

typedef vector<vector<int>> matrix;
ostream& operator<<(ostream& os, const matrix& mt);
void rotateMatrix(const quaternion& q, const matrix& mt, matrix& mt2);
int main()
{
    setlocale(LC_ALL, "Portuguese");
quaternion original=quaternion_from_euler_angles(0,0,90);
quaternion q;
q.setIdentity();
matrix mt={{1,2,3}, {4,5,6}, {7,8,9}};
matrix mt2;
cout<<"Esta é a matriz original..."<<endl<<mt<<endl<<endl;
cout<<"Mostrando a matriz girada por 90 graus para a esquerda..."<<endl;
for(int i=0; i<4; i++)
{
cout<<"Mostrando a matriz girada por +90 graus a esquerda..."<<endl;
q=q*original;
rotateMatrix(q, mt, mt2);
cout<<mt2<<endl;
}
cout<<"Teste finalizado!"<<endl;
return 0;
}

void rotateMatrix(const quaternion& q, const matrix& mt, matrix& mt2)
{
mt2.resize(mt.size());
for(int i=0; i<mt.size(); i++)
{
mt2[i].resize(mt[i].size(), 0);
}
for(int i=0; i<mt.size(); i++)
{
for(int i1=0; i1<mt[i].size(); i1++)
{
vector3d v(i-1, i1-1);
vector3d rf=quaternion_vector_rotate(q, v);
int x=floor(rf.x+1.5);
int y=floor(rf.y+1.5);
mt2[x][y]=mt[i][i1];
}
}
}

ostream& operator<<(ostream& os, const matrix& mt)
{
for(auto& it : mt )
{
for(auto& it2 : it)
{
os<<it2<<" ";
}
os<<endl;
}
return os;
}

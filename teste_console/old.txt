

#include<signal.h>
#include<iostream>
#include<gpp/gpp.h>
#include<gpp_physics/gpp_physics.h>

using namespace std;
using namespace gpp;

atomic<uint32> hstate;
void sig_callback(int x);
ostream& operator<<(ostream& os, const vector<vector<int32>>& mt);
int main()
{
cout.precision(0);
cout<<fixed;
profiler_start();
setlocale(LC_ALL, "Portuguese");
signal(SIGTERM, sig_callback);
signal(SIGINT, sig_callback);
signal(SIGABRT, sig_callback);
float ang=cos((90*3.14)/180);
quaternion q=quaternion_from_euler_angles(0, 0, ang);
vector<vector<int32>> mt={{1,2,3}, {4,5,6}, {7,8,9}};
vector<vector<int32>> mt2;
mt2.resize(3);
for(auto& it : mt2) it.resize(3,0);
for(int i=0; i<3; i++)
{
for(int i1=0; i1<3; i1++)
{
vector3d v(i, i1);
v=quaternion_vector_rotate(q, v);
mt2[v.x][v.y]=mt[i][i1];
}
}
cout<<mt2<<endl;
profiler_dump("profiler_profiler.txt");
return 0;
}

void sig_callback(int x)
{
hstate.store(0);
}

ostream& operator<<(ostream& os, const vector<vector<int32>>& mt)
{
os<<fixed;
os.precision(0);
for(auto& it : mt)
{
for(auto& it2 : it)
{
os<<it2<<' ';
}
os<<endl;
}
os<<endl;
return os;
}

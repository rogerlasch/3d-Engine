
#pragma comment(lib, "D:\\github\\3d_hobby\\gpp\\bin\\gpp.lib")
#pragma comment(lib, "D:\\github\\3d_hobby\\gpp\\dependencies\\libs\\GameNetworkingSockets.lib")
#pragma comment(lib, "D:\\github\\3d_hobby\\gpp\\dependencies\\libs\\bass.lib")
#pragma comment(lib, "gpp_world.lib")
#include<windows.h>
#include<sdl2/sdl.h>
#include"..\\gpp_world.h"
#include"r_window.h"

using namespace std;
using namespace gpp;

void print_bodies(const vector<RigidBody*>& bodies)
{
for(auto& it : bodies)
{
_FLOG("{}", it->toString());
}
}

shared_ptr<r_window> window;
void init_bodies(vector<gpp_zone*>& zones);
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,                      _In_opt_ HINSTANCE hPrevInstance,                      _In_ LPWSTR    lpCmdLine,                      _In_ int       nCmdShow)
{
setlocale(LC_ALL, "Portuguese");
profiler_start();
log_open_file("main.txt");
window=make_shared<r_window>("Main", 0, 0, 800, 800);
vector<gpp_zone*> zones;
init_bodies(zones);
ZoneSpacer zs;
gpp_zone* sf=zs.spatialize(zones);
while(window->is_open())
{
wait_ms(5);
window->make_loop();
}
_FLOG("{}", sf->toString());
delete sf;
vector<gpp_index> arr={
{1, 0, 0}, {2,0,1}, {1,2,0}, {3,2,1}
};
std::sort(arr.begin(), arr.end());
for(auto& it: arr)
{
_FLOG("{}", it.toString());
}
profiler_dump("main_dump.txt");
return 0;
}

void init_bodies(vector<gpp_zone*>& zones)
{
box2d* q1=new box2d({0,0,0}, {500, 500, 0});
box2d* q2=new box2d({20,20,0}, {20, 20, 0});
box2d* q3=new box2d({25,25,0}, {10, 10, 0});
gpp_zone* z1=new gpp_zone();
z1->setName("Mundo");
z1->setIndex({1, 0, GPP_ZONE});
z1->setBody(q1);
gpp_zone* z2=new gpp_zone();
z2->setIndex({2, 0, GPP_ZONE});
z2->setName("Praça");
z2->setBody(q2);
gpp_zone* z3=new gpp_zone();
z3->setIndex({3, 0, GPP_ZONE});
z3->setName("Fonte da praça");
z3->setBody(q3);
zones={z3,z1, z2};
}

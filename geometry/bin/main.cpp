
#pragma comment(lib, "D:\\github\\3d_hobby\\gpp\\bin\\gpp.lib")
#pragma comment(lib, "D:\\github\\3d_hobby\\gpp\\dependencies\\libs\\GameNetworkingSockets.lib")
#pragma comment(lib, "D:\\github\\3d_hobby\\gpp\\dependencies\\libs\\bass.lib")
#pragma comment(lib, "geometry3d.lib")

#include<windows.h>
#include<sdl2/sdl.h>
#include"..\\..\\gpp\\gpp.h"
#include"..\\geometry.h"
#include"r_window.h"

using namespace std;
using namespace gpp;

shared_ptr<r_window> window;
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,                      _In_opt_ HINSTANCE hPrevInstance,                      _In_ LPWSTR    lpCmdLine,                      _In_ int       nCmdShow)
{
setlocale(LC_ALL, "Portuguese");
profiler_start();
log_open_file("main.txt");
try {
line3d l1({0,0,0}, {0,10,0});
line3d l2({10,10,10}, {10,20,10});
while(l2.min.x>=0)
{
vector3d v1, v2;
float f1, f2;
float final=ClosestPtSegmentSegment(l1.min, l1.max, l2.min, l2.max, f1, f2, v1, v2);
_FLOG("\nX:{}", l2.min.x);
_FLOG("Final: {}, f1:{}, f2:{}", final, f1, f2);
_FLOG("V1:{}:{}:{}", v1.x, v1.y, v1.z);
_FLOG("V2:{}:{}:{}", v2.x, v2.y, v2.z);
l2.translate({-1,-1,-1});
}
} catch(const exception& e) {
_FLOG("{}", e.what());
}
window=make_shared<r_window>("Main", 0, 0, 800, 800);
while(window->is_open())
{
wait_ms(5);
window->make_loop();
}
profiler_dump("main_dump.txt");
return 0;
}

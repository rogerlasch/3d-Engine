
#pragma comment(lib, "D:\\github\\3d_hobby\\gpp\\bin\\gpp.lib")
#pragma comment(lib, "D:\\github\\3d_hobby\\gpp\\dependencies\\libs\\GameNetworkingSockets.lib")
#pragma comment(lib, "D:\\github\\3d_hobby\\gpp\\dependencies\\libs\\bass.lib")
#pragma comment(lib, "gpp_game.lib")
#include<windows.h>
#include<sdl2/sdl.h>
#include"..\\gpp_world.h"
#include"r_window.h"

using namespace std;
using namespace gpp;

enum GROUND_TYPES
{
gwhater=0,
gsand,
gconcret,
gconcret1,
ggrass1
};

shared_ptr<r_window> window;
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,                      _In_opt_ HINSTANCE hPrevInstance,                      _In_ LPWSTR    lpCmdLine,                      _In_ int       nCmdShow)
{
setlocale(LC_ALL, "Portuguese");
profiler_start();
log_open_file("main.txt");
window=make_shared<r_window>("Main", 0, 0, 800, 800);
while(window->is_open())
{
wait_ms(5);
window->make_loop();
}
quadtree ts;
ts.create({0,0,0}, 1000000, 256);
_FLOG("Total de nós: {}\nMemória usada:{}", ts.count(), ts.countMemoryUsage());
_FLOG("Profundidade: {}", ts.getDept());
profiler_dump("main_dump.txt");
return 0;
}

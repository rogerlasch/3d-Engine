
#define _INCLUDE_GPP
#include<windowlib.h>

using namespace std;


#define G_TITLE "Main Window"
#define G_WINDOW_WIDTH 1500
#define G_WINDOW_HEIGHT 1500

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
        setlocale(LC_ALL, "Portuguese");
g_window* window=createMainWindow(G_TITLE, 0,0, G_WINDOW_WIDTH, G_WINDOW_HEIGHT);
while(window->is_open())
{
wait_ms(5);
window->make_loop();
}
return 0;
}

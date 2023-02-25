
#include <windows.h>
#include <fstream>
#include <iostream>
#include <memory>
#include "..\\window\\r_window.h"

using namespace std;

shared_ptr<r_window> window;
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
        setlocale(LC_ALL, "Portuguese");
        window = make_shared<r_window>("Main", 0, 0, 1500, 1500);
        window->setBackGroundColor(255, 255, 255);
        while (window->is_open())
        {
                wait_ms(5);
                window->make_loop();
        }
        return 0;
}

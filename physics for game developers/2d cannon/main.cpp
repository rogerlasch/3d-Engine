
#pragma comment(lib, "user32.lib")
#include <windows.h>
#include <fstream>
#include <iostream>
#include <memory>
#include "..\\window\\r_window.h"

using namespace std;


float vm=0.0f;
float   Alpha=0.0f; // Angle from y-axis (upward) to the cannon.
                   // When this angle is 0, the cannon is pointing
            // straight up, when it is 90 degrees, the cannon
                   // is horizontal
float   Gamma=0.0f; // Angle from x-axis, in the x-z plane to the cannon.
                   // When this angle is 0, the cannon is pointing in
                   // the positive x-direction, positive values of this angle
                   // are toward the positive z-axis

float L; // This is the length of the cannon, m
float Yb; // This is the base elevation of the cannon, m
box3d box;
sphere3d s; // The shell position (displacement) vector
float   delta_time;              // The time from the instant the shell leaves
float   tInc;              // the cannon, seconds
                            // The time increment to use when stepping through
                            // the simulation, seconds
float   g; // acceleration due to gravity, m/s^2

shared_ptr<r_window> window;
int DoSimulation(void);
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
profiler_start();
        setlocale(LC_ALL, "Portuguese");
        window = make_shared<r_window>("Main", 0, 0, 1500, 1500);
        window->setBackGroundColor(255, 255, 255);
s.center={50,1000,0};
s.radius=200.0f;
box.min={1000,0,0};
box.measures={300, 50, 50};
bool firing=false;
int64 start=0;
        while (window->is_open())
        {
                wait_ms(5);
                window->make_loop();
if(key_pressed(SDLK_UP)) Alpha+=5.0f;
else if(key_pressed(SDLK_DOWN)) Alpha-=5.0f;
if(key_pressed(SDLK_RIGHT)) Gamma+=5.0f;
else if(key_pressed(SDLK_LEFT)) Gamma-=5.0f;
if(key_pressed('c'))
{
string msg=safe_format("x:{:.0f}, y:{:.0f}", Gamma, Alpha);
MessageBoxA(NULL, msg.c_str(), "info", 0);
}
if(key_pressed(SDLK_RETURN))
{
delta_time=0.0f;
tInc=1.0f;
L=5.0f;
Yb=0.80f;
vm=0.0f;
g=-9.8f;
s.center=(50,1000,0);
firing=true;
start=get_timestamp_ms();
}
if((firing==true)&&(get_timestamp_ms()-start>200))
{
start=get_timestamp_ms();
window->clear(255,255,255);
window->drawCircle(&s, 0,0,0);
window->drawBox(&box, 0,0,0);
_FLOG("{:.2f}:{:.2f}:{:.2f}", s.center.x, s.center.y, s.center.z);
switch(DoSimulation())
{
case 1:
{
string msg=safe_format("Atingiu o alvo no tempo {}", delta_time);
MessageBoxA(NULL, msg.c_str(), "info", 0);
firing=false;
break;
}
case 2:
{
string msg=safe_format("Caiu no chão no tempo {}", delta_time);
MessageBoxA(NULL, msg.c_str(), "info", 0);
firing=false;
break;
}
case 3:
{
string msg=safe_format("Timeout! {:.2f}:{:.2f}", s.center.x, s.center.y);
MessageBoxA(NULL, msg.c_str(), "info", 0);
firing=false;
break;
}
}
}
        }
profiler_dump("profiler.txt");
        return 0;
}

int DoSimulation(void)
{
      double cosX;
      double cosY;
      double cosZ;
      double xe, ze;
      double b, Lx, Ly, Lz;
      double tx1, tx2, ty1, ty2, tz1, tz2;
// step to the next time in the simulation
delta_time+=tInc;
// First calculate the direction cosines for the cannon orientation.
// In a real game, you would not want to put this calculation in this
// function since it is a waste of CPU time to calculate these values
// at each time step as they never change during the sim. We only put them
 // here in this case so you can see all the calculation steps in a single
// function.
b = L * cos((90-Alpha) *3.14/180); // projection of barrel onto x-z plane
Lx = b * cos(Gamma * 3.14/180); // x-component of barrel length
Ly = L * cos(Alpha * 3.14/180); // y-component of barrel length
Lz = b * sin(Gamma * 3.14/180); // z-component of barrel length
cosX = Lx/L;
cosY = Ly/L;
cosZ = Lz/L;
// These are the x and z coordinates of the very end of the cannon barrel
// we'll use these as the initial x and z displacements
xe = L * cos((90-Alpha) *3.14/180) * cos(Gamma * 3.14/180);
ze = L * cos((90-Alpha) *3.14/180) * sin(Gamma * 3.14/180);
// Now we can calculate the position vector at this time
s.center.y = vm * cosX * delta_time + xe;
s.center.x = (Yb + L * cos(Alpha*3.14/180)) + (vm * cosY * delta_time) -
      (0.5 * g * delta_time * delta_time);
s.center.z = vm * cosZ * delta_time + ze;
CollisionData info;
if(collision3d::isColliding(&s, &box, &info))
{
return 1;
}
// Check for collision with ground (x-z plane)
if(s.center.y <= 0)
      return 2;
// Cut off the simulation if it's taking too long
// This is so the program does not get stuck in the while loop
if(delta_time>3600)
      return 3;
if((s.center.x<0)||(s.center.x>1500)||(s.center.y<0)||(s.center.y>1500))
return 3;
            return 0;
      }

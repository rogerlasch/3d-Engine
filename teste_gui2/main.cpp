
#include<windowlib/windowlib.h>
#include<gpp/gpp.h>
#include<gpp_physics/gpp_physics.h>
#include"g_start.h"

using namespace std;
using namespace rgames;
using namespace gpp;

#define G_TITLE "Main Window"
#define G_WINDOW_WIDTH 1500
#define G_WINDOW_HEIGHT 1500
#define G_FILENAME "main_log.txt"

r_window* window=NULL;
RigidBody* ch=NULL;//Player
vector<shared_ptr<iRigidBody>> hbodies;
shared_ptr<gpp_world> game;

void checkKeyboard();
void init_game();
RigidBody* createRandomBody();
RigidBody* createSphere(const vector3d& center, float radius);
RigidBody* createBox(const vector3d& min, const vector3d& mes);
void dispatchCollision(shared_collisionrow& row);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
        setlocale(LC_ALL, "Portuguese");
log_create(G_FILENAME, LG_ALL);
log_set_default(G_FILENAME);
init_game();
window=createMainWindow(G_TITLE, 0,0, G_WINDOW_WIDTH, G_WINDOW_HEIGHT);
int64 x=gpp::get_timestamp_ms();
while(window->is_open())
{
this_thread::sleep_for(chrono::milliseconds(5));
window->update();
checkKeyboard();
if((gpp::get_timestamp_ms()-x)>220){
_GASSERT(game!=NULL);
game->update(0.22f);
x=gpp::get_timestamp_ms();
}
}
return 0;
}

void checkKeyboard()
{
static int64 x=gpp::get_timestamp_ms();
static bool walking=false;
if(rkey_pressed('c')) _speak("{:.2f}:{:.2f}:{:.2f}", ch->position.x, ch->position.y, ch->position.z);
if(rkey_pressed('g')){
vector3d v=quaternion_extract_euler_angles(ch->orientation);
_speak("{:.0f} Graus", v.z);
}
bool front=false;
if(((front=rkey_down(SDLK_UP)))||(rkey_down(SDLK_DOWN))) {
if(!walking){
vector3d ft={0,1,0};
ft=quaternion_vector_rotate(ch->orientation, ft);
if(!front) ft.inverse();
ft*=ch->mass*10.0f;
ch->applyForce(ft);
walking=true;
}
}
if((rkey_up(SDLK_UP))&&(rkey_up(SDLK_DOWN))&&(walking)){
walking=false;
vector3d vdir=ch->velocity;
vdir.normalize();
vdir.inverse();
vdir*=ch->mass*10;
ch->applyForce(vdir);
}

int64 rend=gpp::get_timestamp_ms();
if((rend-x)<220) return;
bool left=false;
if(((left=rkey_down(SDLK_LEFT)))||(rkey_down(SDLK_RIGHT))) {
x=rend;
quaternion qv;
float z=0.0f;
SDL_Keymod mod=SDL_GetModState();
if((mod&KMOD_SHIFT)) z=((left) ? -10 : 10);
else if((mod&KMOD_ALT)) z=((left) ? -45 : 45);
else {
z=((left) ? -1 : 1);
}
qv=quaternion_from_euler_angles(0,0, z);
ch->rotate(ch->position, qv);
}
}

void init_game(){
WorldInfo winfo;
winfo.flags=GWF_GRAVITY;
winfo.min={-100,0,-10000.00f};
winfo.max={1000000, 1000000, 1000000};
winfo.gravity={0.0f, 0.0f, -9.81f};
WorldCallbacks wk;
wk.createDefault();
wk.hcall=dispatchCollision;
game=make_shared<gpp_world>(&winfo, &wk);
RigidBody* ground=createBox(winfo.min, {winfo.max.x, winfo.max.y, fabs(winfo.min.z)});
ground->setName("pedra");
ground->setMass(0.0f);
ground->setRestitution(0.0f);
game->addBody(ground);
RigidBody* b1=createBox({20,20,0}, { 50, 50, 0.0f});
b1->setName("grama");
b1->setMass(0.0f);
b1->setRestitution(0.0f);
game->addBody(b1);
RigidBody* b2=createBox({30,30,0}, { 30, 30, 0.0f});
b2->setName("água");
b2->setMass(0.0f);
b2->setRestitution(0.0f);
game->addBody(b2);
RigidBody* b3=createBox({40,40,0}, {10, 10, 2.0f});
b3->setName("areia");
b3->setMass(0.0f);
b3->setRestitution(0.0f);
game->addBody(b3);
ch=createSphere({2.5f, 2.5f, 2.5f}, 2.5f);
ch->setName("Jogador malvado");
ch->setMass(75.0f);
ch->setRestitution(0.3f);
ch->orientation=quaternion_from_euler_angles(0,0,0);
game->addBody(ch);
}

RigidBody* createRandomBody()
{
static uint32 x=0;
x++;
shared_ptr<RigidBody> rb=make_shared<RigidBody>();
rb->setIndex({x, x+1});
rb->mass=random_float(1.0f, 330.0f);
rb->restitution=random_float(0.01f, 1.0f);
hbodies.push_back(rb);
return rb.get();
}

RigidBody* createSphere(const vector3d& center, float radius){
RigidBody* rb=createRandomBody();
sphere3d* sp=new sphere3d();
sp->center=center;
sp->radius=radius;
rb->setGeometricShape(sp);
return rb;
}

RigidBody* createBox(const vector3d& min, const vector3d& mes){
RigidBody* rb=createRandomBody();
box3d* b=new box3d(min, mes);
rb->setGeometricShape(b);
return rb;
}

void dispatchCollision(shared_collisionrow& row)
{
if(row->at(0)->r1->position==row->at(0)->r1->getGeometricShape()->getLastPosition())return;
if(row->size()>=2){
std::sort(row->begin(), row->end(), [&](const shared_collisioninfo& r1, const
shared_collisioninfo&r2)->bool{
float d1=r1->r1->position*r1->r2->position;
float d2=r2->r1->position*r2->r2->position;
return d1<d2;
});
}
auto& info=row->at(0);
rsound* hsound=window->getSound();
if(info->depth<=0.01f){
string filename="sounds\\steps\\{}{}.ogg";
filename=safe_format(filename, info->r2->name, random_uint32(1,5));
hsound->play(filename);
_GINFO("{}", filename);
}
else{
string filename="sounds\\paredes\\madeira.ogg";
hsound->play(filename);
}
}

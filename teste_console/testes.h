


#ifndef TESTES_H
#define TESTES_H

namespace gpp{
void testeSphereBox()
{
box3d b({0,0,-10}, {100,100,10});
sphere3d s({10,10,-20}, 2.5f);
for(uint32 i=0; i<20; i++)
{
s.translate({0,0,5});
_GINFO("{:.2f}", s.center.z);
CollisionInfo info;
if(collision3d::sphereBox(&s, &b, &info)) {
_GINFO("Colisão: {}", info.toString());
if(info.depth>0)
{
s.translate(info.normal*info.depth);
_GINFO("corrigido: {:.2f}", s.center.z);
}
}
}
}

void testeBoxSphere()
{
box3d b({8,8,8}, {5,5,5});
sphere3d s({10,10,-10}, 10.0f);
for(uint32 i=0; i<20; i++)
{
b.translate({0,0,-2});
vector3d c=b.getCenter();
_GINFO("{:.2f}:{:.2f}:{:.2f}", c.x, c.y, c.z);
CollisionInfo info;
if(collision3d::boxSphere(&b, &s, &info)) {
_GINFO("Colisão: {}", info.toString());
if(info.depth>0)
{
b.translate(info.normal*info.depth);
c=b.getCenter();
_GINFO("{:.2f}:{:.2f}:{:.2f}", c.x, c.y, c.z);
}
}
}
}

/*
void testeSphereCapsule(){
sphere3d sp({0,10,2.5f}, 2.5f);
capsule3d s({25,10,-100}, {25,10,-25}, 25.0f);
s.translate({0,0,10});
while(sp.center.x<75){
sp.translate({1,0,0});
vector3d c=sp.getCenter();
_GINFO("{:.2f}:{:.2f}:{:.2f}", c.x, c.y, c.z);
CollisionInfo info;
if(sphereCapsule(&sp, &s, &info)){
_GINFO("{}", info.toString());
sp.translate(info.normal*info.depth);
vector3d c=sp.getCenter();
_GINFO("{:.2f}:{:.2f}:{:.2f}", c.x, c.y, c.z);
}
}
}
*/
}
#endif

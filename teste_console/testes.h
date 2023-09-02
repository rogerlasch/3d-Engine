


#ifndef TESTES_H
#define TESTES_H

void teste1()
{
box3d b({0,0,-10}, {100,100,10});
sphere3d s({10,10,10}, 2.5f);
for(uint32 i=0; i<20; i++)
{
s.translate({0,0,-2});
_GINFO("{:.2f}", s.center.z);
CollisionInfo info;
if(sphereBox(&s, &b, &info)) {
_GINFO("Colisão: {}", info.toString());
if(info.depth>0)
{
s.translate(info.normal*info.depth);
}
}
}

}
#endif

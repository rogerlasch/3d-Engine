

#include"geometry.h"

using namespace std;

namespace gpp
{
gpp_shape::gpp_shape(uint32 gtype)
{
this->gtype=gtype;
}

gpp_shape:: ~gpp_shape()
{
}

uint32 gpp_shape::getGType()const
{
return this->gtype;
}

gpp_shape* shape_create_from_type(uint32 type, const vector3d& vtranslate)
{
gpp_shape* sh=NULL;
switch(type)
{
case GTYPE_SPHERE:
{
sh=new sphere3d({5.0f, 5.0f, 5.0f}, 5.0f);
break;
}
case GTYPE_BOX:
{
sh=new box3d({0.0f, 0.0f, 0.0f}, {10.0f, 10.0f, 10.0f});
break;
}
case GTYPE_CAPSULE:
case GTYPE_CYLINDER:
case GTYPE_CONE:
case GTYPE_POLYHEDRON:
{
break;
}
case GTYPE_LINE:
{
sh=new line3d({0.0f, 0.0f, 0.0f}, {0.0f, 10.0f, 0.0f});
break;
}
}
if(sh!=NULL)
{
sh->translate(vtranslate);
}
return NULL;
}
}

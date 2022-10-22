

#include"../gpp/gpp.h"
#include"geometry.h"

using namespace std;

namespace gpp
{
shape::shape(uint32 gtype)
{
this->gtype=gtype;
}

shape:: ~shape()
{
}

uint32 shape::getGtype()const
{
return this->gtype;
}
}

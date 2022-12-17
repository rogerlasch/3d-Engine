
#include<string>
#include<map>
#include"gpp_world.h"
#include"types.h"
#include"gpp_ground.h"

using namespace std;

namespace gpp
{
map<uint32, gpp_ground> groundtable;

gpp_ground::gpp_ground()
{
if(groundtable.size()>0)
{
*this=groundtable.begin()->second;
}
else
{
type=0;
name="";
flags=0;
}
}

gpp_ground::gpp_ground(uint32 type, const string& name, uint32 flags)
{
this->type=type;
this->name=name;
this->flags=flags;
}

gpp_ground::gpp_ground(const gpp_ground& g)
{
*this=g;
}

gpp_ground& gpp_ground::operator=(const gpp_ground& g)
{
this->type=g.type;
this->name=g.name;
this->flags=g.flags;
return *this;
}

gpp_ground::~gpp_ground()
{
}

void gpp_ground::setType(uint32 type)
{
this->type=type;
}

uint32 gpp_ground::getType()const
{
return this->type;
}

void gpp_ground::setFlags(uint32 flags)
{
this->flags=flags;
}

uint32 gpp_ground::getFlags()const
{
return this->flags;
}

void gpp_ground::setName(const string& name)
{
this->name=name;
}

string gpp_ground::getName()const
{
return this->name;
}

//Funções que manipulam um objeto gpp_ground.
//Estas funções são independentes da classe.

bool gpp_ground_register(uint32 type, const string& name, uint32 flags)
{
if(groundtable.find(type)==groundtable.end())
{
gpp_ground g(type, name, flags);
groundtable.insert(make_pair(type, g));
return true;
}
return false;
}

bool gpp_ground_unregister(uint32 type)
{
auto it=groundtable.find(type);
if(it!=groundtable.end())
{
groundtable.erase(it);
return true;
}
return false;
}

bool gpp_ground_get(uint32 type, gpp_ground* g)
{
auto it=groundtable.find(type);
if(it!=groundtable.end())
{
if(g!=NULL)
{
*g=it->second;
}
return true;
}
return false;
}
}

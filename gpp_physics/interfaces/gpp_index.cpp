
#include<sstream>
#include<vector>
#include"../types.h"
#include"gpp_index.h"

using namespace std;

namespace gpp
{

gpp_index::gpp_index()
{
vnum=0;
subvnum=0;
}

gpp_index::gpp_index(uint32 vnum, uint32 subvnum)
{
this->vnum=vnum;
this->subvnum=subvnum;
}

gpp_index::gpp_index(const initializer_list<uint32>& id)
{
*this=id;
}

gpp_index::gpp_index(const gpp_index& di)
{
*this=di;
}

gpp_index& gpp_index::operator=(const initializer_list<uint32>& id)
{
vector<uint32> d=id;
if(d.size()==0)
{
return *this;
}
setVnum(d[0]);
if(d.size()>1)
{
setSubVnum(d[1]);
}
return *this;
}

gpp_index& gpp_index::operator=(const gpp_index& di)
{
this->vnum=di.vnum;
this->subvnum=di.subvnum;
return *this;
}

bool gpp_index::operator<(const gpp_index& di)const
{
if(this->vnum<di.vnum)
{
return true;
}
return this->subvnum<di.subvnum;
}

bool gpp_index::operator==(const gpp_index& di)const
{
return this->vnum==di.vnum&&this->subvnum==di.subvnum;
}

void gpp_index::set(uint32 vnum, uint32 subvnum)
{
this->vnum=vnum;
this->subvnum=subvnum;
}

void gpp_index::setVnum(uint32 vnum)
{
this->vnum=vnum;
}

void gpp_index::setSubVnum(uint32 subvnum)
{
this->subvnum=subvnum;
}

uint32 gpp_index::getVnum()const
{
return this->vnum;
}

uint32 gpp_index::getSubVnum()const
{
return this->subvnum;
}

string gpp_index::toString()const
{
stringstream ss;
ss<<"Vnum: "<<vnum<<", SubVnum: "<<subvnum;
return ss.str();
}
}

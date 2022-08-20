
#include<initializer_list>
#include<vector>
#include<dlb_utils/dlb_types.h>
#include"dlb_index.h"

using namespace std;

namespace dlb
{
dlb_index::dlb_index(uint32 vnum, uint32 subvnum)
{
this->vnum=vnum;
this->subvnum=subvnum;
}

dlb_index::dlb_index(const initializer_list<uint32>& vns)
{
*this=vns;
}

dlb_index::dlb_index(const dlb_index& di)
{
*this=di;
}

dlb_index& dlb_index::operator=(const dlb_index& di)
{
this->vnum=di.vnum;
this->subvnum=di.subvnum;
return *this;
}

dlb_index& dlb_index::operator=(const initializer_list<uint32>& vns)
{
vector<uint32> arr(vns);
if(arr.size()>2)
{
arr.resize(2);
}
switch(arr.size())
{
case 1:
{
this->vnum=arr[0];
this->subvnum=dlb_invalid_vnum;
break;
}
case 2:
{
this->vnum=arr[0];
this->subvnum=arr[1];
break;
}
default:
{
this->vnum=dlb_invalid_vnum;
this->subvnum=dlb_invalid_vnum;
break;
}
}
return *this;
}
bool dlb_index::operator<(const dlb_index& di)const
{
if(this->vnum==di.vnum)
{
return this->subvnum<di.subvnum;
}
return this->vnum<di.vnum;
}

bool dlb_index::operator==(const dlb_index& di)const
{
return this->vnum==di.vnum&&this->subvnum==di.subvnum;
}

void dlb_index::set(uint32 vnum, uint32 subvnum)
{
this->vnum=vnum;
this->subvnum=subvnum;
}

void dlb_index::setvnum(uint32 vnum)
{
this->vnum=vnum;
}

void dlb_index::set_subvnum(uint32 subvnum)
{
this->subvnum=subvnum;
}

uint32 dlb_index::getvnum()const
{
return this->vnum;
}

uint32 dlb_index::get_subvnum()const
{
return this->subvnum;
}
}

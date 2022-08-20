
#include"../dlb_utils/dlb_types.h"
#include"dlb_index.h"

using namespace std;

namespace dlb
{
dlb_index::dlb_index(uint32 vnum, uint32 subvnum)
{
this->vnum=vnum;
this->subvnum=subvnum;
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

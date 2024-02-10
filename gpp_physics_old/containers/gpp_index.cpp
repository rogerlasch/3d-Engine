
#include<sstream>
#include<vector>
#include"../common.h"
#include"MurmurHash3.h"
#include"gpp_index.h"

using namespace std;

namespace gpp
{
const uint32_t FNV_PRIME = 16777619;
const uint32_t FNV_OFFSET_BASIS = 2166136261;


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

uint32 IndexHasher::operator()(const gpp_index& id)const
{
uint64 hash = FNV_OFFSET_BASIS;
    hash ^= id.vnum;
    hash *= FNV_PRIME;
    hash ^= id.subvnum;
    hash *= FNV_PRIME;
return hash;
}

uint64 get_hash_from_index(const gpp_index& id1, const gpp_index& id2) {
uint32 arr [4]={id1.vnum, id2.subvnum, id1.subvnum, id2.vnum};
int32 seed=160320394;
uint64 hash=0;
MurmurHash3_x64_128 (arr, sizeof(arr), seed, &hash);
    return hash;
}
}

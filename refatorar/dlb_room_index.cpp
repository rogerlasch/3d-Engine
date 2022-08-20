

#include"../dlb_utils.h"
#include"dlb_basic_data.h"
#include"dlb_floor.h"
#include"dlb_room.h"
#include"dlb_room_index.h"

using namespace std;

namespace dlb
{
dlb_room_index::dlb_room_index()
{
room_index_init();
}

dlb_room_index::~dlb_room_index()
{
room_index_free();
}

uint32 dlb_room_index::getvnum()const
{
return this->index.getvnum();
}

uint32 dlb_room_index::count()const
{
return ((this->rooms.size()>1) ? this->rooms.size()-1 : 0);
}

dlb_room* dlb_room_index::get_master()const
{
if(this->rooms.size()==0)
{
return NULL;
}
return this->rooms.begin()->second;
}

dlb_room* dlb_room_index::get_room(uint32 subvnum)const
{
if((subvnum==dlb_invalid_vnum)||(this->rooms.count(subvnum)==0))
{
return NULL;
}
return this->rooms.at(subvnum);
}

//seters
void dlb_room_index::setvnum(uint32 vnum)
{
this->index.setvnum(vnum);
for(auto it=this->rooms.begin(); it!=this->rooms.end(); ++it)
{
it->second->setvnum(vnum);
}
}

//Outros métodos
dlb_room* dlb_room_index::create_copy()
{
if(this->rooms.size()==0)
{
return NULL;
}
dlb_room* r=this->get_master();
dlb_room* room=new dlb_room();
room->setvnum(r->getvnum());
room->set_subvnum(this->generate_subvnum());
room->set_basicflags(r->get_basicflags());
room->set_basic_type(r->get_basic_type());
room->settype(r->gettype());
room->setflags((r->getflags()^basic_master|basic_copyable));
room->setname(r->getname());
room->set_shortdesc(r->get_shortdesc());
room->set_longdesc(r->get_longdesc());
room->setarea(r->getarea());
room->setfloor(r->getfloor());
room->parent=NULL;
room->rooms.clear();
this->rooms.insert(make_pair(room->get_subvnum(), room));
return room;
}

bool dlb_room_index::destroy_copy(uint32 subvnum)
{
if((this->rooms.size()<2)||(this->rooms.count(subvnum)==0))
{
return false;
}
delete this->rooms.at(subvnum);
this->rooms.erase(subvnum);
return true;
}

void dlb_room_index::room_index_init()
{
if(this->rooms.size()>0)
{
return;
}
this->current_subvnum=0;
this->index.set(0, 0);
this->rooms.clear();
dlb_room* room=new dlb_room();
room->setvnum(0);
room->set_basicflags(basic_master|basic_resizable|basic_copyable|basic_fixed);
this->rooms.insert(make_pair(0, room));
}

void dlb_room_index::room_index_free()
{
if(this->rooms.size()==0)
{
return;
}
for(auto it=this->rooms.begin(); it!=this->rooms.end(); ++it)
{
if(it->second->is_master())
{
continue;
}
//it->second->area->remove_room
delete it->second;
}
delete this->rooms.begin()->second;
this->rooms.clear();
}

uint32 dlb_room_index::generate_subvnum()
{
this->current_subvnum++;
return this->current_subvnum;
}

}

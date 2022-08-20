

#ifndef DLB_ROOM_INDEX_H
#define DLB_ROOM_INDEX_H

#include<map>
namespace dlb
{

class dlb_room_index
{
private:
uint32 current_subvnum;
dlb_index index;
std::map<uint32, dlb_room*> rooms;
public:
dlb_room_index();
dlb_room_index(const dlb_room_index& dr)=delete;
dlb_room_index& operator=(const dlb_room_index& dr)=delete;
~dlb_room_index();
uint32 getvnum()const;
uint32 count()const;
dlb_room* get_master()const;
dlb_room* get_room(uint32 subvnum)const;
//seters
void setvnum(uint32 vnum);
//Outros métodos
dlb_room* create_copy();
bool destroy_copy(uint32 subvnum);
private:
void room_index_init();
void room_index_free();
uint32 generate_subvnum();
};
}
#endif

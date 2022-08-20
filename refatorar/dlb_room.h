

#ifndef DLB_ROOM_H
#define DLB_ROOM_H

namespace dlb
{

enum dlb_room_flags
{
room_closed=1<<0,
room_reverb=1<<1,
room_safe=1<<2,
room_pk=1<<3,
room_nofloor=1<<4,
room_primary=1<<5
};

class dlb_room : public dlb_basic_data
{
private:
std::string area;//Área atual que a sala se encontra.
dlb_doublevector range;
dlb_floor floor;
dlb_room* parent;//A sala pai
std::list<dlb_room*> rooms;//As salas filhas.
public:
dlb_room();
dlb_room(const dlb_room& dr)=delete;
dlb_room& operator=(const dlb_room& dr)=delete;
~dlb_room();
dlb_doublevector getrange()const;
dlb_floor getfloor()const;
std::string getarea()const;
dlb_room* getparent()const;
uint32 get_rooms(std::list<dlb_room*>& rooms)const;
//seters
void setarea(const std::string& area);
void setrange(const dlb_doublevector& range);
void setfloor(const dlb_floor& floor);
//outros
bool destroy_room(const dlb_index& di);
private:
void room_init();
void room_free();
void extract_to_room(dlb_room* to_room, dlb_room* from_room);
friend class dlb_room_index;
};

}
#endif

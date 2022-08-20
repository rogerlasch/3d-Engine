
/**
*dlb_area.h
*Usado para armazenar dados de configurações de certas regiões
*nome, nome de arquivo, descrição, créditos, e coisas semelhantes.
**/
#ifndef DLB_AREA_H
#define DLB_AREA_H

namespace dlb
{
enum dlb_area_flags
{
area_1d=1<<0,
area_2d=1<<1,
area_3d=1<<2,
area_primary=1<<3
};

class dlb_area : public dlb_basic_data
{
private:
std::string filename;
std::string authors;
std::string credits;
std::string msg_reset;
uint32 low_vnum;
uint32 high_vnum;
uint32 time_reset;
dlb_doublevector range;
dlb_area* parent;
std::list<dlb_area*> areas;
public:
dlb_area();
dlb_area(const dlb_area& da)=delete;
dlb_area& operator=(const dlb_area& da)=delete;
~dlb_area();
//geters
std::string get_filename()const;
std::string get_authors()const;
std::string get_credits()const;
std::string get_msg_reset()const;
uint32 get_low_vnum()const;
uint32 get_high_vnum()const;
void get_vnums(uint32* low_vnum, uint32* high_vnum)const;
uint32 get_time_reset()const;
dlb_doublevector get_range()const;
dlb_area* getparent()const;
uint32 getareas(std::list<dlb_area*>& areas)const;
//seters
void set_filename(const std::string& filename);
void set_authors(const std::string& authors);
void set_credits(const std::string& credits);
void set_msg_reset(const std::string& msg_reset);
void set_vnums(uint32 low_vnum, uint32 high_vnum);
void set_time_reset(uint32 time_reset);
void set_range(const dlb_doublevector& range);
void setparent(const dlb_area* area);
void add_area(const dlb_area* area);
//outros
std::string generate_header();
private:
void area_init();
void area_free();
void extract_to_area(dlb_area* to_area, dlb_area* from_area);
};
}
#endif


#include"../dlb_utils.h"
#include"dlb_basic_data.h"
#include"dlb_area.h"

using namespace std;

namespace dlb
{
dlb_area::dlb_area():dlb_basic_data()
{
this->area_init();
}

dlb_area::~dlb_area()
{
this->area_free();
}

//geters
string dlb_area::get_filename()const
{
return this->filename;
}

string dlb_area::get_authors()const
{
return this->authors;
}

string dlb_area::get_credits()const
{
return this->credits;
}

string dlb_area::get_msg_reset()const
{
return this->msg_reset;
}

uint32 dlb_area::get_low_vnum()const
{
return this->low_vnum;
}

uint32 dlb_area::get_high_vnum()const
{
return this->high_vnum;
}

void dlb_area::get_vnums(uint32* low_vnum, uint32* high_vnum)const
{
if(low_vnum!=NULL)
{
*low_vnum=this->low_vnum;
}
if(high_vnum!=NULL)
{
*high_vnum=this->high_vnum;
}
}

uint32 dlb_area::get_time_reset()const
{
return this->time_reset;
}

dlb_doublevector dlb_area::get_range()const
{
return this->range;
}

dlb_area* dlb_area::getparent()const
{
return this->parent;
}

uint32 dlb_area::getareas(std::list<dlb_area*>& areas)const
{
areas.clear();
for(list<dlb_area*>::const_iterator it=this->areas.begin(); it!=this->areas.end(); ++it)
{
areas.push_back(*it);
}
return this->areas.size();
}

//seters
void dlb_area::set_filename(const string& filename)
{
this->filename=filename;
}

void dlb_area::set_authors(const string& authors)
{
this->authors=authors;
}

void dlb_area::set_credits(const string& credits)
{
this->credits=credits;
}

void dlb_area::set_msg_reset(const string& msg_reset)
{
this->msg_reset=msg_reset;
}

void dlb_area::set_vnums(uint32 low_vnum, uint32 high_vnum)
{
this->low_vnum=low_vnum;
this->high_vnum=high_vnum;
}

void dlb_area::set_time_reset(uint32 time_reset)
{
this->time_reset=time_reset;
}

void dlb_area::set_range(const dlb_doublevector& range)
{
this->range=range;
}

string dlb_area::generate_header()
{
stringstream ofn;
ofn<<"#area header"<<endl;
ofn<<"name "<<getname()<<endl;
ofn<<"authors "<<this->get_authors()<<endl;
ofn<<"credits "<<this->get_credits()<<endl;
ofn<<"msgreset "<<this->get_msg_reset()<<endl;
ofn<<"time_reset "<<get_time_reset()<<endl;
ofn<<"vnums "<<this->get_low_vnum()<<" "<<this->get_high_vnum()<<endl;
//ofn<<"coordinates "<<this->get_range().to_string()<<endl;
return ofn.str();
}

//outros

void dlb_area::area_init()
{
if(this->areas.size()>0)
{
return;
}
this->filename="";
this->authors="";
this->credits="";
this->msg_reset="";
this->low_vnum=dlb_invalid_vnum;
this->high_vnum=dlb_invalid_vnum;
this->time_reset=0;
this->range.set(0, 0, 0, 0, 0, 0);
}

void dlb_area::area_free()
{
for(list<dlb_area*>::iterator it=areas.begin(); it!=areas.end(); ++it)
{
if(((this->get_basicflags()&basic_destroy_cascade))||((this->getflags()&area_primary))||(this->parent==NULL))
{
(*it)->parent=NULL;
delete (*it);
}
else
{
extract_to_area(this->parent, *it);
}
}
this->parent=NULL;
areas.clear();
}

void dlb_area::extract_to_area(dlb_area* to_area, dlb_area* from_area)
{
if(from_area==NULL)
{
return;
}
if(to_area==NULL)
{
from_area->parent=NULL;
delete from_area;
return;
}
from_area->parent=to_area;
to_area->areas.push_back(from_area);
}
}

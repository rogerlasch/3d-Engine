
#include"dlb_basic_data.h"

using namespace std;

namespace dlb
{
dlb_basic_data::dlb_basic_data()
{
this->cleanup();
}

void dlb_basic_data::cleanup()
{
this->index.set(0, 0);
this->basic_flags=0;
this->basic_type=basic_invalid;
this->type=basic_invalid;
this->name="";
this->shortdesc="";
this->longdesc="";
}

//seters
void dlb_basic_data::setindex(const dlb_index& di)
{
this->index=index;
}

bool dlb_basic_data::setvnum(const uint32 vnum)
{
this->index.vnum=vnum;
return true;
}

/**
*Se o objeto for o objeto mestre, ele não poderá ter seu subvnum alterado.
*Não modifique as basic_flags para permitir isso, ou coisas realmente desagradáveis poderão acontecer.
**/
bool dlb_basic_data::set_subvnum(uint32 subvnum)
{
if(this->is_master())
{
return false;
}
this->index.subvnum=subvnum;
return true;
}

/**
*Define o tipo base do objeto.
*Por exemplo, se foi um basic_obj, basic_char ou uma basic_room que herdou esta classe.
*Serve para identificar a qual subclasse o objeto pertence.
*Use só as constantes basic_types para isto.
**/
bool dlb_basic_data::set_basic_type(uint32 basic_type)
{
if((basic_type<=basic_invalid)||(basic_type>=basic_last))
{
return false;
}
this->basic_type=basic_type;
return true;
}

/**
*Define o tipo secundário do objeto.
*Isto depende para qual basic_class for direcionado.
*Por exemplo, se basic_type for basic_char, este valor pode ser npc ou player
*Também se basic_type for basic_obj, este valor pode ser obj_weapon, obj_treasure, obj_container, ... Etc...
**/
void dlb_basic_data::settype(uint32 type)
{
this->type=type;
}

void dlb_basic_data::setname(const string& name)
{
this->name=name;
}

void dlb_basic_data::set_basicflags(uint64 basic_flags)
{
this->basic_flags=((basic_flags==0) ? 0 : this->basic_flags|basic_flags);
}

void dlb_basic_data::setflags(uint64 flags)
{
this->flags=((flags==0) ? 0 : this->flags|flags);
}

void dlb_basic_data::set_shortdesc(const string& shortdesc)
{
this->shortdesc=shortdesc;
}

void dlb_basic_data::set_longdesc(const string& longdesc)
{
this->longdesc=longdesc;
}

//geters
uint32 dlb_basic_data::getvnum()const
{
return this->index.vnum;
}

uint32 dlb_basic_data::get_subvnum()const
{
return this->index.subvnum;
}

dlb_index dlb_basic_data::getindex()const
{
return this->index;
}

uint32 dlb_basic_data::get_basic_type()const
{
return this->basic_type;
}

uint32 dlb_basic_data::gettype()const
{
return this->type;
}

uint64 dlb_basic_data::get_basicflags()const
{
return this->basic_flags;
}

uint64 dlb_basic_data::getflags()const
{
return this->flags;
}

string dlb_basic_data::getname()const
{
return this->name;
}

string dlb_basic_data::get_shortdesc()const
{
return shortdesc;
}

string dlb_basic_data::get_longdesc()const
{
return this->longdesc;
}

//outros métodos

 bool dlb_basic_data::is_indestructible()const
{
return (this->basic_flags&basic_indestructible)==basic_indestructible;
}

 bool dlb_basic_data::is_blocking()const
{
return (this->basic_flags&basic_blocking)==basic_blocking;
}

 bool dlb_basic_data::is_passable()const
{
return (this->basic_flags&basic_nopass)==0;
}

 bool dlb_basic_data::is_scalable()const
{
return (this->basic_flags&basic_scalable)==basic_scalable;
}

 bool dlb_basic_data::is_master()const
{
return (this->basic_flags&basic_master)==basic_master;
}

 bool dlb_basic_data::is_fixed()const
{
return (this->basic_flags&basic_fixed)==basic_fixed;
}

 bool dlb_basic_data::is_copyable()const
{
return (this->basic_flags&basic_copyable)==basic_copyable;
}

 bool dlb_basic_data::is_resizable()const
{
return (this->basic_flags&basic_resizable)==basic_resizable;
}

 bool dlb_basic_data::is_npc()const
{
return this->basic_type==basic_mob;
}

 bool dlb_basic_data::is_player()const
{
return this->basic_type==basic_player;
}

 bool dlb_basic_data::is_wall()const
{
return this->basic_type==basic_wall;
}

 bool dlb_basic_data::is_object()const
{
return this->basic_type==basic_obj;
}

 bool dlb_basic_data::is_exit()const
{
return this->basic_type==basic_exit;
}

 bool dlb_basic_data::is_room()const
{
return this->basic_type==basic_room;
}

//Ação
//Ação padrão do objeto, sem parâmetros.
//Por segurança, ela sempre falhará, a menos que seja sobrescrita em outra classe.
bool dlb_basic_data::interact()
{
return false;
}

//Ação padrão do objeto, com um objeto somente leitura
//Por padrão, ela sempre falhará a menos que seja sobrescrita em outra classe.
bool dlb_basic_data::interact(const dlb_basic_data& db)
{
return false;
}

//Ação padrão do objeto, com um objeto que pode ser modificado.
//Por padrão, ela sempre falhará a menos que seja sobrescrita em outra classe.
bool dlb_basic_data::interact(dlb_basic_data* dba)
{
return false;
}
}



/**
*Classe base para tudo o que vier depois.
*npcs, objetos, ve�culos, etc...
*Adiciona comportamento padr�o caso os m�todos n�o sejam sobrescritos.
**/

#ifndef DLB_BASIC_DATA_H
#define DLB_BASIC_DATA_H

#include<string>
#include<sstream>
#include"../dlb_utils/dlb_measure.h"
#include"../dlb_utils/dlb_vector.h"
#include"../dlb_utils/dlb_doublevector.h"
#include"../dlb_utils/dlb_types.h"
#include"dlb_index.h"

namespace dlb
{
/**
*Algumas flags de comportamento que qualquer objeto deve ter
*N�o importa se � uma bala voando, um npc, ou um player.
*Todas classes implementam essas flags.
**/
enum data_basic_flags
{
basic_blocking=1<<0,//o objeto est� bloqueando a passagem
basic_nopass=1<<1,//O objeto n�o permite a passagem de forma alguma.
basic_scalable=1<<2,//O objeto pode ser escalado
basic_indestructible=1<<3,//O objeto n�o pode ser destru�do.
basic_master=1<<4, //O objeto � o primeiro daquele vnum a ser constru�do
basic_fixed=1<<5, //O objeto n�o pode ser movido.
basic_resizable=1<<6, //O objeto pode ser redimensionado
basic_copyable=1<<7, //O objeto pode ser copiado
basic_destroy_cascade=1<<8 //Remove os objetos filhos junto com o objeto pai, se n�o, os mesmos ser�o extra�dos para um n�vel superior
};

//Tipos que derivar�o desta classe.
enum data_basic_types
{
basic_invalid=0,//N�o use, apenas para verifica��o.
basic_mob=1,//O objeto � um npc
basic_player,//� um jogador
basic_wall,//� uma parede
basic_room, //� uma sala
basic_world, //� o mundo
basic_obj, // � um objeto.
basic_area,//� um objeto de �rea
basic_exit, //� uma sa�da para outra sala.
basic_last //N�o use, apenas para verifica��o interna.
};

class dlb_basic_data
{
private:
dlb_index index;//Vnum e subvnum Usado por sala, wall, objetos e praticamente todo o resto
uint64 basic_flags;//flags b�sicas dos objetos
uint64 flags;
uint32 basic_type;//Tipo base do objeto
uint32 type;// Tipo secund�rio do objeto. Se basic_type for basic_obj, este tipo pode ser mochila, arma, sof�, tanto faz. Depende dos subtipos.
std::string name;//nome do objeto
std::string shortdesc;//descri��o curta que aparecer� quando se olhar algo de longe
std::string longdesc;//descri��o completa que aparecer� quando se olhar algo de perto
public:
dlb_basic_data();
dlb_basic_data(const dlb_basic_data& obj)=delete;
dlb_basic_data& operator=(const dlb_basic_data& obj)=delete;
virtual void cleanup();
//seters
void setindex(const dlb_index& di);
bool setvnum(const uint32 vnum);
bool set_subvnum(uint32 subvnum);
bool set_basic_type(uint32 basic_type);
void settype(uint32 type);
void setname(const std::string& name);
void set_basicflags(uint64 basic_flags=0);
void setflags(uint64 flags);
void set_shortdesc(const std::string& shortdesc);
void set_longdesc(const std::string& longdesc);
//geters
uint32 getvnum()const;
uint32 get_subvnum()const;
dlb_index getindex()const;
uint32 get_basic_type()const;
uint32 gettype()const;
uint64 get_basicflags()const;
uint64 getflags()const;
std::string getname()const;
std::string get_shortdesc()const;
std::string get_longdesc()const;
//outros m�todos
virtual bool is_indestructible()const;
virtual bool is_blocking()const;
virtual bool is_passable()const;
virtual bool is_scalable()const;
virtual bool is_resizable()const;
virtual bool is_npc()const;
virtual bool is_player()const;
virtual bool is_wall()const;
virtual bool is_object()const;
virtual bool is_exit()const;
virtual bool is_room()const;
virtual bool is_master()const;
virtual  bool is_fixed()const;
virtual bool is_copyable()const;
//A��o
virtual bool interact();//Intera��o padr�o do objeto.
virtual bool interact(const dlb_basic_data& db);//Itera��o padr�o com objeto somente leitura
virtual bool interact(dlb_basic_data* dba);//Intera��o padr�o com objeto que pode ser modificado.
};
}
#endif

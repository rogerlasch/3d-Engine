

/**
*Classe base para tudo o que vier depois.
*npcs, objetos, veículos, etc...
*Adiciona comportamento padrão caso os métodos não sejam sobrescritos.
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
*Não importa se é uma bala voando, um npc, ou um player.
*Todas classes implementam essas flags.
**/
enum data_basic_flags
{
basic_blocking=1<<0,//o objeto está bloqueando a passagem
basic_nopass=1<<1,//O objeto não permite a passagem de forma alguma.
basic_scalable=1<<2,//O objeto pode ser escalado
basic_indestructible=1<<3,//O objeto não pode ser destruído.
basic_master=1<<4, //O objeto é o primeiro daquele vnum a ser construído
basic_fixed=1<<5, //O objeto não pode ser movido.
basic_resizable=1<<6, //O objeto pode ser redimensionado
basic_copyable=1<<7, //O objeto pode ser copiado
basic_destroy_cascade=1<<8 //Remove os objetos filhos junto com o objeto pai, se não, os mesmos serão extraídos para um nível superior
};

//Tipos que derivarão desta classe.
enum data_basic_types
{
basic_invalid=0,//Não use, apenas para verificação.
basic_mob=1,//O objeto é um npc
basic_player,//é um jogador
basic_wall,//É uma parede
basic_room, //é uma sala
basic_world, //é o mundo
basic_obj, // é um objeto.
basic_area,//É um objeto de área
basic_exit, //é uma saída para outra sala.
basic_last //Não use, apenas para verificação interna.
};

class dlb_basic_data
{
private:
dlb_index index;//Vnum e subvnum Usado por sala, wall, objetos e praticamente todo o resto
uint64 basic_flags;//flags básicas dos objetos
uint64 flags;
uint32 basic_type;//Tipo base do objeto
uint32 type;// Tipo secundário do objeto. Se basic_type for basic_obj, este tipo pode ser mochila, arma, sofá, tanto faz. Depende dos subtipos.
std::string name;//nome do objeto
std::string shortdesc;//descrição curta que aparecerá quando se olhar algo de longe
std::string longdesc;//descrição completa que aparecerá quando se olhar algo de perto
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
//outros métodos
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
//Ação
virtual bool interact();//Interação padrão do objeto.
virtual bool interact(const dlb_basic_data& db);//Iteração padrão com objeto somente leitura
virtual bool interact(dlb_basic_data* dba);//Interação padrão com objeto que pode ser modificado.
};
}
#endif

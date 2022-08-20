
#include<map>
#include"../dlb_utils.h"
#include"dlb_floor.h"

using namespace std;

namespace dlb
{
//Tabela de pisos padrão.
static map<uint32, string> floor_table={
{floor_default, "default"},
{floor_grama_curta, "grama curta"},
{floor_grama_longa, "grama longa"},
{floor_madeira, "madeira"},
{floor_metal, "metal"},
{floor_lama, "lama"},
{floor_terra, "terra"},
{floor_agua_rasa, "água rasa"},
{floor_agua_profunda, "água profunda"},
{floor_areia, "areia"},
{floor_neve_rasa, "neve rasa"},
{floor_neve_profunda, "neve profunda"},
{floor_ar, "ar"},
{floor_concreto, "concreto"},
{floor_pedra, "pedra"}
};

dlb_floor::dlb_floor()
{
this->type=floor_default;
dlb_floor_load_name(this);
this->flags=0;
this->range.set(0, 0, 0, 0, 0, 0);
}

dlb_floor::dlb_floor(const dlb_floor& df)
{
*this=df;
}

dlb_floor& dlb_floor::operator=(const dlb_floor& df)
{
this->type=df.type;
this->name=df.name;
this->flags=df.flags;
this->range=df.range;
return *this;
}

bool dlb_floor::operator==(const uint32 type)const
{
return this->type==type;
}

bool dlb_floor::operator==(const std::string& name)const
{
return this->name==name;
}

void dlb_floor::cleanup()
{
this->type=0;
this->name="";
this->flags=0;
this->range.set(0, 0, 0, 0, 0, 0);
}

//geters
uint32 dlb_floor::gettype()const
{
return this->type;
}

float dlb_floor::get_ladder_height()const
{
return max_ladder_height;
}

uint64 dlb_floor::getflags()const
{
return this->flags;
}

string dlb_floor::getname()const
{
return this->name;
}

dlb_doublevector dlb_floor::getrange()const
{
return this->range;
}

//seters
/**
*Sertifique-se de estar inserindo os valores corretamente. Estas funções não fazem nenhum tipo de validação de parâmetros. Estas funções servirão para tornar o objeto thread safe futuramente.
*A função settype define o tipo passado como parâmetro e imediatamente tenta carregar o nome descritivo cujo o tipo representa.
*os tipos válidos podem ser consultados a cima no objeto table_floor.
**/
void dlb_floor::settype(uint32 type)
{
this->type=type;
dlb_floor_load_name(this);
}

void dlb_floor::set_ladder_height(float ladder_height)
{
if(!this->is_ladder())
{
this->max_ladder_height=0.0f;
this->range.resize_z(this->range.get_z1(), 0.0f);
return;
}
if(ladder_height<0)
{
return;
}
this->max_ladder_height=ladder_height;
this->resize_ladder();
}

void dlb_floor::setflags(uint64 flags)
{
this->flags=flags;
}

/**
*Esta função define os limites de um piso.
*Normalmente, esta função é usada por dlb_wall e dlb_zone, é recomendável não chamar este método diretamente.
**/
void dlb_floor::setrange(const dlb_doublevector& dv)
{
this->range.set(dv.get_x1(), dv.get_x2(), dv.get_y1(), dv.get_y2(), dv.get_z1(), dv.get_z2());
this->resize_ladder();
}

bool dlb_floor::is_ladder()const
{
return this->flags&floor_ladder;
}

void dlb_floor::resize_ladder()
{
if(!this->is_ladder())
{
this->max_ladder_height=0.0f;
this->range.set_z(this->range.get_z1(), 0.0f);
return;
}
if(this->range.get_height()>max_ladder_height)
{
this->range.resize_z(this->range.get_z1(), max_ladder_height);
}
}

//Funções que manipulam um objeto dlb_floor.
//Estas funções são independentes da classe.

/**
*Esta função irá tentar carregar o nome descritivo do piso apontado por type
*existe uma sobrecarga que aceita só o objeto de piso, para determinar qual é o tipo e carregar o nome.
*Esta função atribui o type ao floor.type e depois retorna invocando a sobrecarga.
*Esta função retornará false se o tipo não corresponder ou se o objeto for nulo.
**/
bool dlb_floor_load_name(dlb_floor* df, uint32 floor_type)
{
if(df==NULL)
{
return false;
}
df->type=floor_type;
return dlb_floor_load_name(df);
}

bool dlb_floor_load_name(dlb_floor* df)
{
if(df==NULL)
{
return false;
}
if(floor_table.count(df->type)==0)
{
return false;
}
df->name=floor_table.at(df->type);
return true;
}

/**
*Verifica se um piso especificado existe na tabela de pisos.
*true se existir, false caso contrário.
**/
bool dlb_floor_exists(uint32 type)
{
return floor_table.count(type)>0;
}
}

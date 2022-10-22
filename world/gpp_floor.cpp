
#include<string>
#include<map>
#include"types.h"
#include"gpp_floor.h"

using namespace std;

namespace gpp
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

gpp_floor::gpp_floor()
{
this->type=floor_default;
gpp_floor_load_name(this);
this->flags=0;
}

gpp_floor::gpp_floor(const gpp_floor& df)
{
*this=df;
}

gpp_floor& gpp_floor::operator=(const gpp_floor& df)
{
this->type=df.type;
this->name=df.name;
this->flags=df.flags;
return *this;
}

bool gpp_floor::operator==(const uint32 type)const
{
return this->type==type;
}

bool gpp_floor::operator==(const std::string& name)const
{
return this->name==name;
}

void gpp_floor::cleanup()
{
this->type=0;
this->name="";
this->flags=0;
}

//geters
uint32 gpp_floor::gettype()const
{
return this->type;
}

uint32 gpp_floor::getflags()const
{
return this->flags;
}

string gpp_floor::getname()const
{
return this->name;
}

//seters
/**
*Sertifique-se de estar inserindo os valores corretamente. Estas funções não fazem nenhum tipo de validação de parâmetros. Estas funções servirão para tornar o objeto thread safe futuramente.
*A função settype define o tipo passado como parâmetro e imediatamente tenta carregar o nome descritivo cujo o tipo representa.
*os tipos válidos podem ser consultados a cima no objeto table_floor.
**/
void gpp_floor::settype(uint32 type)
{
this->type=type;
gpp_floor_load_name(this);
}

void gpp_floor::setflags(uint32 flags)
{
this->flags=flags;
}

//Funções que manipulam um objeto gpp_floor.
//Estas funções são independentes da classe.

/**
*Esta função irá tentar carregar o nome descritivo do piso apontado por type
*existe uma sobrecarga que aceita só o objeto de piso, para determinar qual é o tipo e carregar o nome.
*Esta função atribui o type ao floor.type e depois retorna invocando a sobrecarga.
*Esta função retornará false se o tipo não corresponder ou se o objeto for nulo.
**/
bool gpp_floor_load_name(gpp_floor* df, uint32 floor_type)
{
if(df==NULL)
{
return false;
}
df->type=floor_type;
return gpp_floor_load_name(df);
}

bool gpp_floor_load_name(gpp_floor* df)
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
bool gpp_floor_exists(uint32 type)
{
return floor_table.count(type)>0;
}
}

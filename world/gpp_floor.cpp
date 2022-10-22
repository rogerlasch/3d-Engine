
#include<string>
#include<map>
#include"types.h"
#include"gpp_floor.h"

using namespace std;

namespace gpp
{
//Tabela de pisos padr�o.
static map<uint32, string> floor_table={
{floor_default, "default"},
{floor_grama_curta, "grama curta"},
{floor_grama_longa, "grama longa"},
{floor_madeira, "madeira"},
{floor_metal, "metal"},
{floor_lama, "lama"},
{floor_terra, "terra"},
{floor_agua_rasa, "�gua rasa"},
{floor_agua_profunda, "�gua profunda"},
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
*Sertifique-se de estar inserindo os valores corretamente. Estas fun��es n�o fazem nenhum tipo de valida��o de par�metros. Estas fun��es servir�o para tornar o objeto thread safe futuramente.
*A fun��o settype define o tipo passado como par�metro e imediatamente tenta carregar o nome descritivo cujo o tipo representa.
*os tipos v�lidos podem ser consultados a cima no objeto table_floor.
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

//Fun��es que manipulam um objeto gpp_floor.
//Estas fun��es s�o independentes da classe.

/**
*Esta fun��o ir� tentar carregar o nome descritivo do piso apontado por type
*existe uma sobrecarga que aceita s� o objeto de piso, para determinar qual � o tipo e carregar o nome.
*Esta fun��o atribui o type ao floor.type e depois retorna invocando a sobrecarga.
*Esta fun��o retornar� false se o tipo n�o corresponder ou se o objeto for nulo.
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
*true se existir, false caso contr�rio.
**/
bool gpp_floor_exists(uint32 type)
{
return floor_table.count(type)>0;
}
}

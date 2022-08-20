

#include"../dlb_utils.h"
#include"dlb_basic_data.h"
#include"dlb_floor.h"
#include"dlb_room.h"

using namespace std;

namespace dlb
{
dlb_room::dlb_room() : dlb_basic_data()
{
room_init();
}

dlb_room::~dlb_room()
{
this->room_free();
}

dlb_doublevector dlb_room::getrange()const
{
return this->range;
}

dlb_floor dlb_room::getfloor()const
{
return this->floor;
}

string dlb_room::getarea()const
{
return this->area;
}

dlb_room* dlb_room::getparent()const
{
return this->parent;
}

uint32 dlb_room::get_rooms(std::list<dlb_room*>& rooms)const
{
rooms.clear();
for(list<dlb_room*>::const_iterator it=this->rooms.begin(); it!=this->rooms.end(); ++it)
{
rooms.push_back(*it);
}
return this->rooms.size();
}

//seters
void dlb_room::setarea(const string& area)
{
this->area=area;
}

void dlb_room::setrange(const dlb_doublevector& range)
{
this->range=range;
this->floor.setrange(this->range);
}

void dlb_room::setfloor(const dlb_floor& floor)
{
this->floor=floor;
}

void dlb_room::room_init()
{
dlb_basic_data::settype(basic_room);
dlb_basic_data::set_basicflags(0);
dlb_basic_data::setflags(0);
dlb_basic_data::setname("Flutuando no vazio");
dlb_basic_data::set_shortdesc("Um vazio profundo se estende para todos os lados.");
dlb_basic_data::set_longdesc("Voc� vaga desorientado pelo Interplano sem rumo e sem destino. Criaturas\nbizarras e monstruosas passam diante de voc� a caminho de outros planos.\nSeres estranhos com olhos vermelhos em meio as trevas lhe observam com\nolhares famintos e maldosos. Sombras perversas lhe rodeam, querendo se\nalimentar da sua ess�ncia. ");
this->floor.setrange(this->range);
this->parent=NULL;
this->rooms.clear();
}

/**
*Esta fun��o � chamada automaticamente pelo destruidor da classe. N�o altere
*esta fun��o a menos que voc� saiba o que est� fazendo, e tamb�m n�o chame esta fun��o sem ser no destruidor.
*Esta fun��o ir� destruir ou realocar suas salas filhos.
*caso a propriedade parente seja ==NULL, as salas de n�vel inferior ser�o destru�das.
*Voc� tamb�m consegue este efeito especificando a flag basic_destroy_cascad na propriedade basicflags.
*Caso o parente de n�vel superior seja v�lido, e a flag basic_destroy_cascad n�o estiver presente, as salas filhos ser�o atribu�das para a sala de n�vel superior.
*Isto continuar� at� que o parente de n�vel superior seja NULL, alguma sala tenha a basic_destroy_cascad especificada, ou ainda, se alguma sala tiver a flag room_primary especificada. Esta flag  sinaliza que aquela sala � a primeira daquela �rea.
*Ent�o, todas salas de n�vel inferior ser�o efetivamente destruidas.
**/
void dlb_room::room_free()
{
if(this->rooms.size()>0)
{
for(auto it=rooms.begin(); it!=rooms.end(); ++it)
{
if(((get_basicflags()&basic_destroy_cascade))||((this->getflags()&room_primary))||(this->parent==NULL))
{
(*it)->parent=NULL;
delete (*it);
}
else
{
this->extract_to_room(this->parent, *it);
}
}
}
parent=NULL;
rooms.clear();
}

/**
*Este m�todo ir� extrair uma sala para uma sala de n�vel superior.
*Por exemplo, voc� tem 3 salas, sala 1, 2 e 3. A sala 1 � pai da sala 2 que � pai da sala 3.
*A sala 2 precisa ser removida, mas n�o existe problema se a sala 3 continuar existindo.
*Ent�o, esta fun��o adiciona a sala 3 na lista de filhos da sala 1 j� que a sala 2 deixar� de existir em breve e se a sala 3 n�o for realocada, o comportamento ser� indefinido.
*Aten��o para alguns comportamentos especiais
*caso a sala 1 for a primeira sala da �rea, ou, a sala 2 tiver a propriedade parent==NULL, ent�o a dele��o da sala 3 ser� for�ada.
*Caso a sala 3 tenha filhos tamb�m, e nenhum parente de n�vel superior puder ser encontrado, isto gerar� um efeito cascata de dele��o at� que n�o aja mais salas de n�vel inferior para serem exclu�das.
**/
void dlb_room::extract_to_room(dlb_room* to_room, dlb_room* from_room)
{
if(from_room==NULL)
{
return;
}
if(to_room==NULL)
{
from_room->parent=NULL;
delete from_room;
return;
}
from_room->parent=to_room;
to_room->rooms.push_back(from_room);
}
}

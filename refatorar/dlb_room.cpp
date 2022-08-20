

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
dlb_basic_data::set_longdesc("Você vaga desorientado pelo Interplano sem rumo e sem destino. Criaturas\nbizarras e monstruosas passam diante de você a caminho de outros planos.\nSeres estranhos com olhos vermelhos em meio as trevas lhe observam com\nolhares famintos e maldosos. Sombras perversas lhe rodeam, querendo se\nalimentar da sua essência. ");
this->floor.setrange(this->range);
this->parent=NULL;
this->rooms.clear();
}

/**
*Esta função é chamada automaticamente pelo destruidor da classe. Nâo altere
*esta função a menos que você saiba o que está fazendo, e também não chame esta função sem ser no destruidor.
*Esta função irá destruir ou realocar suas salas filhos.
*caso a propriedade parente seja ==NULL, as salas de nível inferior serão destruídas.
*Você também consegue este efeito especificando a flag basic_destroy_cascad na propriedade basicflags.
*Caso o parente de nível superior seja válido, e a flag basic_destroy_cascad não estiver presente, as salas filhos serão atribuídas para a sala de nível superior.
*Isto continuará até que o parente de nível superior seja NULL, alguma sala tenha a basic_destroy_cascad especificada, ou ainda, se alguma sala tiver a flag room_primary especificada. Esta flag  sinaliza que aquela sala é a primeira daquela área.
*Então, todas salas de nível inferior serão efetivamente destruidas.
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
*Este método irá extrair uma sala para uma sala de nível superior.
*Por exemplo, você tem 3 salas, sala 1, 2 e 3. A sala 1 é pai da sala 2 que é pai da sala 3.
*A sala 2 precisa ser removida, mas não existe problema se a sala 3 continuar existindo.
*Então, esta função adiciona a sala 3 na lista de filhos da sala 1 já que a sala 2 deixará de existir em breve e se a sala 3 não for realocada, o comportamento será indefinido.
*Atenção para alguns comportamentos especiais
*caso a sala 1 for a primeira sala da área, ou, a sala 2 tiver a propriedade parent==NULL, então a deleção da sala 3 será forçada.
*Caso a sala 3 tenha filhos também, e nenhum parente de nível superior puder ser encontrado, isto gerará um efeito cascata de deleção até que não aja mais salas de nível inferior para serem excluídas.
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

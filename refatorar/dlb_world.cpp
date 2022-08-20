
#include<map>
#include<mutex>
#include"../dlb_utils.h"
#include"dlb_basic_data.h"
#include"dlb_floor.h"
#include"dlb_area.h"
#include"dlb_obj.h"
#include"dlb_wall.h"
#include"dlb_mob.h"
#include"dlb_player.h"
#include"dlb_room.h"
#include"dlb_world.h"

using namespace std;

namespace dlb
{
static bool world_loaded=false;
static dlb_doublevector world_range;
static map<uint32, dlb_mob*> mobs;
static map<uint32, dlb_obj*> objs;
static map<string, dlb_player*> players;
static map<uint32, dlb_wall*> walls;
static map<uint32, dlb_room*> rooms;
static map<string, dlb_area*> areas;
//Mutexs...
static mutex mtx_mobs;
static mutex mtx_objs;
static mutex mtx_players;
static mutex mtx_walls;
static mutex mtx_rooms;
static mutex mtx_areas;


//Funções

/**
*Esta função deve ser chamada antes de construir seu mapa.
*Esta função cria a área principal, cria os objetos indispensáveis e também alguns npcs.
*se caso range for NULL, o tamanho padrão é usado.
*O tamanho mínimo para um dos lados é de 1000000. Caso algum dos 3 eixos for menor que isto, um ajuste será aplicado automaticamente.
*retorna true se for bem sucedida ou false se alguma falha ocorrer.
*No momento o que pode causar uma falha é se caso o mundo não tenha cido limpo antes.
**/
bool dlb_world_init(dlb_doublevector* range)
{
if(dlb_world_is_loaded())
{
return false;
}
if(range==NULL)
{
world_range.set(0, 4000000000, 0, 4000000000, -1000000, 1000000000);
}
else
{
if(range->get_width()<1000000)
{
range->set_x(0, 1000000);
}
if(range->get_length()<1000000)
{
range->set_y(0, 1000000);
}
if(range->get_height()<1000000)
{
range->set_z(-100000, 900000);
}
if(range->get_z1()>0)
{
range->set_z(0, range->get_z2());
}
world_range=*range;
}

world_loaded=true;
return true;
}

/**
*Esta função deve ser chamada antes do encerramento do seu jogo. Seria bom se ela fosse a última função a ser chamada antes de sair.
*Ela é responsável por salvar jogadores, destruir objetos, salas, npcs, áreas e tudo mais liberando a memória.
*Caso você faça uma limpeza de tempos em tempos sem fechar o jogo, lembre-se que deverá chamar dlb_world_init antes de recriar suas áreas novamente.
**/
void dlb_world_cleanup()
{
if(world_loaded==false)
{
return;
}
dlb_world_players_cleanup();
dlb_world_objs_cleanup();//Libera todos os objetos
dlb_world_mobs_cleanup();//Remove todos os npcs do mundo.
dlb_world_walls_cleanup();//Libera todas paredes.
dlb_world_rooms_cleanup();//Libera todas salas
dlb_world_areas_cleanup();//Libera todas as áreas
world_range.set(0, 0, 0, 0, 0, 0);
world_loaded=false;
}

/**
*Verifica se o mundo foi inicializado corretamente.
*Caso não tenha sido inicializado, a maioria das funções de criação irão falhar.
**/
bool dlb_world_is_loaded()
{
return world_loaded;
}

//Liberar memória
/**
*As próximas funções são responsáveis por limpar toda a memória usada pelos objetos.
*Retorna o número de objetos destruídos, ou 0 se nada for apagado.
**/
uint32 dlb_world_mobs_cleanup()//Remove todos os npcs do mundo.
{
unique_lock<mutex> lck(mtx_mobs);
if(mobs.size()==0)
{
return 0;
}
uint32 deleteds=mobs.size();
while(mobs.size()>0)
{
delete mobs.begin()->second;
mobs.erase(mobs.begin());
}
return deleteds;
}

uint32 dlb_world_objs_cleanup()//Libera todos os objetos
{
unique_lock<mutex> lck(mtx_objs);
if(objs.size()==0)
{
return 0;
}
uint32 deleteds=objs.size();
while(objs.size()>0)
{
delete objs.begin()->second;
objs.erase(objs.begin());
}
return deleteds;
}

uint32 dlb_world_players_cleanup()
{
unique_lock<mutex> lck(mtx_players);
if(players.size()==0)
{
return 0;
}
uint32 deleteds=players.size();
while(players.size()>0)
{
delete players.begin()->second;
players.erase(players.begin());
}
return deleteds;
}

uint32 dlb_world_walls_cleanup()//Libera todas paredes.
{
unique_lock<mutex> lck(mtx_walls);
if(walls.size()==0)
{
return 0;
}
uint32 deleteds=walls.size();
while(walls.size()>0)
{
delete walls.begin()->second;
walls.erase(walls.begin());
}
return deleteds;
}

uint32 dlb_world_rooms_cleanup()//Libera todas salas
{
unique_lock<mutex> lck(mtx_rooms);
if(rooms.size()==0)
{
return 0;
}
uint32 deleteds=rooms.size();
while(rooms.size()>0)
{
delete rooms.begin()->second;
rooms.erase(rooms.begin());
}
return deleteds;
}

uint32 dlb_world_areas_cleanup()//Libera todas as áreas
{
unique_lock<mutex> lck(mtx_areas);
if(areas.size()==0)
{
return 0;
}
uint32 deleteds=areas.size();
while(areas.size()>0)
{
delete areas.begin()->second;
areas.erase(areas.begin());
}
return deleteds;
}

//contadores
/**
*Estas funções retornam a quantidade de objetos no mundo.
*se vnum for igual a dlb_invalid_vnum, a contagem de todos objetos distintos no mundo é retornada.
*Se um vnum for especificado e ele for válido, o número de cópias daquele objeto é retornado.
*Algumas funções não levam parâmetros, como é o caso da dlb_world_players_count.
*Neste caso o número de jogadores no mundo é retornado.
*Se alguma função falhar por vnum inválido, ou por não existir nada semelhante, 0 é retornado.
**/
uint32 dlb_world_mob_count(uint32 vnum)
{
if(mobs.size()==0)
{
return 0;
}
if(vnum==dlb_invalid_vnum)
{
return mobs.size();
}
return ((mobs.count(vnum)==0) ? 0 : mobs.at(vnum)->count());
}

uint32 dlb_world_obj_count(uint32 vnum)
{
if(objs.size()==0)
{
return 0;
}
if(vnum==dlb_invalid_vnum)
{
return objs.size();
}
return ((objs.count(vnum)==0) ? 0 : objs.at(vnum)->count());
}

uint32 dlb_world_player_count()
{
return players.size();
}

uint32 dlb_world_wall_count(uint32 vnum)
{
if(walls.size()==0)
{
return 0;
}
if(vnum==dlb_invalid_vnum)
{
return walls.size();
}
return ((walls.count(vnum)==0) ? 0 : walls.at(vnum)->count());
}

uint32 dlb_world_room_count(uint32 vnum)
{
if(rooms.size()==0)
{
return 0;
}
if(vnum==dlb_invalid_vnum)
{
return rooms.size();
}
return ((rooms.count(vnum)==0) ? 0 : rooms.at(vnum)->count());
}

uint32 dlb_world_area_count()
{
return areas.size();
}

//World manager

/**
*Esta função é responsável por não permitir que uma área, sala ou qualquer
*outra coisa seja maior que o mundo, ou que esteja fora de seus limites.
*Caso esteja fora dos limites, o limite mais próximo será aplicado, o que pode dar problemas.
**/
void dlb_world_verife_range(dlb_doublevector* range)
{
float f1=0, f2=0, f3=0, f4=0, f5=0, f6=0;
range->get(&f1, &f2, &f3, &f4, &f5, &f6);
if(f1<world_range.get_x1())
{
f1=world_range.get_x1();
}
if(f2>world_range.get_x2())
{
f2=world_range.get_x2();
}
if(f3<world_range.get_y1())
{
f3=world_range.get_y1();
}
if(f4>world_range.get_y2())
{
f4=world_range.get_y2();
}
if(f5<world_range.get_z1())
{
f5=world_range.get_z1();
}
if(f6>world_range.get_z2())
{
f6=world_range.get_z2();
}
range->set(f1, f2, f3, f4, f5, f6);
}

//Vnum manager

/**
*As próximas funções operam sobre vnums.
*Vnums são identificadores únicos para cada tipo de objeto, não importa se é
*npc, objeto parede, ou sala.
*2 objetos não podem ter o mesmo vnum, mas 2 objetos de classes  diferente podem ter o mesmo vnum.
*Por exemplo, duas paredes não podem compartilhar o mesmo vnum, mas uma parede e uma sala podem ter o mesmo vnum por serem coisas diferentes.
**/

/**
*Verifica se um vnum é válido.
*Retorna false se o vnum for == a dlb_invalid_vnum, ou maior que dlb_max_vnum.
**/
bool dlb_vnum_is_invalid(uint32 vnum)
{
return vnum==dlb_invalid_vnum||vnum>dlb_max_vnum;
}

/**
*Verifica se o vnum está fora da faixa da área especificada.
*Por segurança, caso a área não seja encontrada, será retornado true.
*Por exemplo, a área especificada não existe, mas não quer dizer que não exista outra área ocupando esta faixa.
**/
bool dlb_vnum_out_of_range(const std::string& mapname, uint32 vnum)
{
dlb_area* dm=dlb_area_get(mapname);
if(dm==NULL)
{
return true;
}
return vnum>=dm->get_low_vnum()&&vnum<=dm->get_high_vnum();
}

/**
*Verifica se o vnum especificado já está sendo utilizado.
*Caso o vnum especificado não seja da faixa da área especificada, a função irá retornar true para prevenir conflitos.
*Retorna true se estiver em uso, ou false se estiver livre.
**/
bool dlb_vnum_in_use(uint32 vnum_type, uint32 vnum)
{
if(dlb_vnum_is_invalid(vnum))
{
return true;
}
switch(vnum_type)
{
case basic_obj:
{
return objs.count(vnum)>0;
}
case basic_mob:
{
return mobs.count(vnum)>0;
}
case basic_wall:
{
return walls.count(vnum)>0;
}
case basic_room:
{
return rooms.count(vnum)>0;
}
}
return true;
}

/**
*Verifica se uma faixa de vnums já está em uso.
*Normalmente isto é verificado no momento da criação de uma nova área.
*Para evitar problemas, se uma faixa inválida for passada, a função retornará true para não permitir o uso desta faixa.
*Retorna true se a faixa de vnums estiver em uso ou for inválida, ou false se estiver livre.
**/
bool dlb_vnum_range_in_use(const std::string& mapname, uint32 low_vnum, uint32 high_vnum)
{
if(low_vnum>high_vnum)
{
uint32 x=low_vnum;
low_vnum=high_vnum;
high_vnum=x;
}
if((dlb_vnum_is_invalid(low_vnum))||(dlb_vnum_is_invalid(high_vnum)))
{
return true;
}
//Verifica se o intervalo de vnums não entra em conflito com nenhum outro mapa.
for(auto it=areas.begin(); it!=areas.end(); ++it)
{
dlb_area* dm=it->second;
if(it->first==mapname)
{
continue;
}
if((dm->get_low_vnum()<=low_vnum)&&(dm->get_high_vnum()>=low_vnum)||(dm->get_low_vnum()<=high_vnum)&&( dm->get_high_vnum()>=high_vnum))
{
return true;
}
}
return false;
}

//Area manager

/**
*Verifica se uma área existe.
**/
bool dlb_area_exists(const string& name)
{
return areas.count(name)>0;
}

/**
*Cria uma nova área.
*O name será usado tanto para o nome em si, como o nome do arquivo da área. Claro que poderá nomear o arquivo como quiser quando salvar.
*Verifique o parâmetro range após a criação da área para sertificar-se de que os limites estão corretos. Caso algum limite esteja fora dos limites do mundo, o mesmo é redimensionado para servir no mundo.
*O nome da área é verificado para verificar se não existem caracteres estranhos.
*O nome é convertido para minúsculo também.
*Caso a função falhe, NULL será retornado.
*Caso contrário, um ponteiro para a nova área é retornado.
**/
dlb_area* dlb_area_create(const string& name, dlb_doublevector* range, uint32 low_vnum, uint32 high_vnum, uint64 flags)
{
string name_cleared=dlb_string_verife(name);
name_cleared=dlb_string_to_lower_case(name_cleared);
if(dlb_area_exists(name))
{
return NULL;
}
if(range==NULL)
{
return NULL;
}
if(dlb_vnum_range_in_use(name_cleared, low_vnum, high_vnum))
{
return NULL;
}
dlb_world_verife_range(range);
if((range->get_width()==0)||(range->get_length()==0)||(range->get_height()==0))
{
return NULL;
}
dlb_area* area=new dlb_area();
area->setname(name_cleared);
area->set_filename(name_cleared);
area->set_vnums(low_vnum, high_vnum);
area->set_range(*range);
area->set_time_reset(30);
area->set_msg_reset("Default reset message");
areas.insert(make_pair(name_cleared, area));
return area;
}

/**
*Destrói uma área sem salvar.
*
**/
bool dlb_area_destroy(const string& name)
{
return false;
}

/**
*Recupera um ponteiro para uma área criada.
*Cuidado deve ser tomado ao manipular uma área diretamente via ponteiro, ou coisas imprevisíveis poderão acontecer.
**/
dlb_area* dlb_area_get(const string& name)
{
if(areas.count(name)==0)
{
return NULL;
}
return areas.at(name);
}

//Room manager

/**
*Verifica se uma sala existe.
*se o vnum estiver na faixa válida entre 1 e dlb_max_vnum,
*E o subvnum for 0, retorna true se a sala mestre foi criada e existe, ou false se não existir nenhuma sala mestre com o vnum especificado.
*Quando o vnum for válido e o subvnum for diferente de 0,
*É verificado se a sala mestre existe. Caso exista, verifica se a sala tem uma cópia com o subvnum especificado.
*Caso ambos sejam verdadeiros, true é retornado.
**/
bool dlb_room_exists(const dlb_index& rindex)
{
if(dlb_vnum_is_invalid(rindex.getvnum())
{
return false;
}
if(rindex.get_subvnum()==0)
{
return rooms.count(rindex.get_vnum()>0;
}
if(rooms.count(rindex.getvnum())==0)
{
return false;
}
return rooms.at(rindex.getvnum())->rooms.count(rindex.get_subvnum())>0;
}

/**
*Esta função destrói uma sala ou uma cópia.
*Se o subvnum for 0, a sala mestra será destruída, destruindo também todas as cópias que foram feitas dela.
*Se um subvnum for especificado, a sala com o subvnum informado será destruída.
*No momento o comportamento de objetos, npcs  e players é indefinido.
**/
bool dlb_room_destroy(const dlb_index& rindex)
{
//Verifica se a sala existe
if(!dlb_room_exists(rindex))
{
return false;
}
switch(rindex.get_subvnum())
{
case 0:
{
dlb_room* room=rooms.at(rindex.getvnum());
delete room;
rooms.erase(rindex.getvnum());
return true;
}
default:
{
dlb_room* room=rooms.at(index.getvnum());
return room->destroy_room(index.get_subvnum());
}
}
return false;
}

/**
*Cria uma nova sala, ou copia uma existente.
*se create_copy for true, uma cópia de uma sala existente será feita e será alocada na área especificada pelo parâmetro area_name.
*Se create_copy for false, uma nova sala será criada se estiver dentro da faixa e vnums da área especificada.
*se create_copy for true, a função falhará nos seguintes casos: Se a sala mestre não existir, se a sala mestre não tiver a flag basic_master, se a área de destino não existir, ou se não tiver a flag basic_copyable.
*Se create_copy for false a função irá falhar nos seguintes casos: Se o vnum especificado já estiver em uso ou for inválido, ou se a área especificada não existir.
*retorna um ponteiro para a nova sala, ou a nova cópia em sucesso, ou NULL em falha.
**/
dlb_room* dlb_room_create(uint32 rvnum, const std::string& area_name, bool create_copy=false)
{
//A área não existe.
if(!dlb_area_exists(area_name))
{
return NULL;
}
if(dlb_vnum_in_use(basic_room, rvnum))
{
return NULL;
}
return NULL;
}
//dlb_room* dlb_room_get(const dlb_index& rindex)

}

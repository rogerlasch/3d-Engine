

/**
*Fun��es para manipular o mundo.
*O mundo n�o � nada mais que um objeto dlb_doublevector com limites gigantescos onde pode ser constru�do mais �reas.
*Caracter�sticas principais::
*Ele sempre dever� ser configurado antes que qualquer �rea ou sala seja criada.
*Mais de um caminho para o mesmo mapa.
*Por exemplo, em mapas de �udio games hoje em dia, voc� � obrigado a passar por dentro de determinada �rea para chegar em uma outra.
*Tendo um mundo como raiz, voc� pode contornar a cidade at� chegar onde deseja, tornando as coisas um pouco mais realistas.
*O n�vel do ch�o � na coordenada 0, sempre. A baixo disso � considerado subterr�neo, e a cima espa�o aberto.
*Ao menos por aqui, e por enquanto, nosso mundo � plano! :D
**/
#ifndef DLB_WORLD_H
#define dlb_WORLD_H

#if !defined(DLB_UTILS_H)
#include"../dlb_utils.h"
#endif
#include"dlb_index.h"
#include"dlb_basic_data.h"
#include"dlb_floor.h"
#include"dlb_room.h"
#include"dlb_room_index.h"
#include"dlb_area.h"

namespace dlb
{

bool dlb_world_init(dlb_area* adata=NULL);//Inicializa o sistema
void dlb_world_cleanup();//Destr�i tudo o que foi criado e carregado.
bool dlb_world_is_loaded();//Verifica se j� foi inicializado.
//Liberar mem�ria
uint32 dlb_world_mobs_cleanup();//Remove todos os npcs do mundo.
uint32 dlb_world_objs_cleanup();//Libera todos os objetos
uint32 dlb_world_players_cleanup();
uint32 dlb_world_walls_cleanup();//Libera todas paredes.
uint32 dlb_world_rooms_cleanup();//Libera todas salas
uint32 dlb_world_areas_cleanup();//Libera todas as �reas
//contadores
uint32 dlb_world_mob_count(uint32 vnum=dlb_invalid_vnum);
uint32 dlb_world_obj_count(uint32 vnum=dlb_invalid_vnum);
uint32 dlb_world_player_count();
uint32 dlb_world_wall_count(uint32 vnum=dlb_invalid_vnum);
uint32 dlb_world_room_count(uint32 vnum=dlb_invalid_vnum);
uint32 dlb_world_area_count();
//World manager
void dlb_world_verife_range(dlb_doublevector* range);
//Vnum manager
bool dlb_vnum_is_invalid(uint32 vnum);//==0 ou maior que dlb_max_vnum
bool dlb_vnum_out_of_range(const std::string& mapname, uint32 vnum);
bool dlb_vnum_in_use(uint32 vnum_type, uint32 vnum);
bool dlb_vnum_range_in_use(const std::string& mapname, uint32 low_vnum, uint32 high_vnum);
//area manager
bool dlb_area_exists(const std::string& name);
bool dlb_area_destroy(const std::string& name);
dlb_area* dlb_area_create(const std::string& name, dlb_doublevector* range, uint32 low_vnum, uint32 high_vnum, uint64 flags);
dlb_area* dlb_area_get(const std::string& name);
//Room manager
bool dlb_room_exists(const dlb_index& rindex);
bool dlb_room_destroy(const dlb_index& rindex);//Destroy uma sala eliminando todas as c�pias, ou destroy apenas uma das c�pias.
dlb_room* dlb_room_create(uint32 rvnum, const std::string& area_name, bool create_copy=false);
dlb_room* dlb_room_get(const dlb_index& rindex);

}
#endif

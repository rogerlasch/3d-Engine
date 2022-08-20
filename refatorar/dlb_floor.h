

/**
*Classe que representa o chão a ser pisado.
*situações onde pode ser usado, no momento: em salas e no topo de paredes.
*No momento existem alguns pisos só para testes. Futuramente poderão ser adicionados mais.
*Você pode carregar um modelo e adaptar o comportamento do mesmo para o que precisar.
**/

#ifndef DLB_FLOOR_H
#define DLB_FLOOR_H

namespace dlb
{

//Estes são todos os pisos disponíveis no momento.
enum dlb_floor_types
{
floor_default=0,
floor_grama_curta,
floor_grama_longa,
floor_madeira,
floor_metal,
floor_lama,
floor_terra,
floor_agua_rasa,
floor_agua_profunda,
floor_areia,
floor_neve_rasa,
floor_neve_profunda,
floor_ar,
floor_concreto,
floor_pedra
};

//Estas são as flags que definirão o comportamento do piso quando um jogador/npc/qualquer outra coisa tentar interagir.
enum dlb_floor_flags
{
floor_ladder=1,//O piso é uma escada que permite subir e descer. Também depende do mapa ser 1, 2 ou 3d.
floor_dig=2,//O piso pode ser escavado
floor_smooth=4,//O chão é escorregadio, algo como estar pisando no gelo.
floor_hot=8,//O chão é quente, queimaduras podem acontecer.
floor_mud=16//O chão é lama, por tanto, objetos soltos podem afundar.
};

class dlb_floor
{
public:
uint32 type;//O tipo numérico do piso.
float max_ladder_height;
uint64 flags;//As flags que definem o comportamento do piso
std::string name;//O nome descritivo do piso. É definido pela propriedade type a cima.
dlb_doublevector range;
dlb_floor();
dlb_floor(const dlb_floor& df);
dlb_floor& operator=(const dlb_floor& df);
bool operator==(const uint32 type)const;
bool operator==(const std::string& name)const;
bool operator==(const dlb_vector& df)const;
bool operator==(const dlb_floor& df)const;
void cleanup();
//geters
uint32 gettype()const;
float get_ladder_height()const;
uint64 getflags()const;
std::string getname()const;
dlb_doublevector getrange()const;
//seters
void settype(uint32 type);
void set_ladder_height(float ladder_height);
void setflags(uint64 flags);
void setrange(const dlb_doublevector& dv);
bool is_ladder()const;
void resize_ladder();
};

bool dlb_floor_load_name(dlb_floor* df, uint32 floor_type);
bool dlb_floor_load_name(dlb_floor* df);
bool dlb_floor_exists(uint32 type);
}
#endif

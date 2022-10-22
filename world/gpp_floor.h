

/**
*Classe que representa o ch�o a ser pisado.
*situa��es onde pode ser usado, no momento: em salas e no topo de paredes.
*No momento existem alguns pisos s� para testes. Futuramente poder�o ser adicionados mais.
*Voc� pode carregar um modelo e adaptar o comportamento do mesmo para o que precisar.
**/

#ifndef GPP_FLOOR_H
#define GPP_FLOOR_H

namespace gpp
{

//Estes s�o todos os pisos dispon�veis no momento.
enum gpp_floor_types
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

//Estas s�o as flags que definir�o o comportamento do piso quando um jogador/npc/qualquer outra coisa tentar interagir.
enum gpp_floor_flags
{
floor_ladder=1,//O piso � uma escada que permite subir e descer. Tamb�m depende do mapa ser 1, 2 ou 3d.
floor_dig=2,//O piso pode ser escavado
floor_smooth=4,//O ch�o � escorregadio, algo como estar pisando no gelo.
floor_hot=8,//O ch�o � quente, queimaduras podem acontecer.
floor_mud=16//O ch�o � lama, por tanto, objetos soltos podem afundar.
};

class gpp_floor
{
public:
uint32 type;//O tipo num�rico do piso.
uint32 flags;//As flags que definem o comportamento do piso
std::string name;//O nome descritivo do piso. � definido pela propriedade type a cima.
gpp_floor();
gpp_floor(const gpp_floor& df);
gpp_floor& operator=(const gpp_floor& df);
bool operator==(const uint32 type)const;
bool operator==(const std::string& name)const;
bool operator==(const gpp_floor& df)const;
void cleanup();
//geters
uint32 gettype()const;
uint32 getflags()const;
std::string getname()const;
//seters
void settype(uint32 type);
void setflags(uint32 flags);
};

bool gpp_floor_load_name(gpp_floor* df, uint32 floor_type);
bool gpp_floor_load_name(gpp_floor* df);
bool gpp_floor_exists(uint32 type);
}
#endif

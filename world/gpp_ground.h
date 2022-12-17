

/**
*Classe que representa o chão a ser pisado.
*situações onde pode ser usado, no momento: em salas e no topo de paredes.
*No momento existem alguns pisos só para testes. Futuramente poderão ser adicionados mais.
*Você pode carregar um modelo e adaptar o comportamento do mesmo para o que precisar.
**/

#ifndef GPP_GROUND_H
#define GPP_GROUND_H

namespace gpp
{

class gpp_ground
{
public:
uint32 type;
uint32 flags;
std::string name;
gpp_ground();
gpp_ground(const gpp_ground& g);
gpp_ground(uint32 type, const std::string& name, uint32 flags);
gpp_ground& operator=(const gpp_ground& g);
virtual ~gpp_ground();
void setType(uint32 type);
uint32 getType()const;
void setName(const std::string& name);
std::string getName()const;
void setFlags(uint32 flags);
uint32 getFlags()const;
};

bool gpp_ground_register(uint32 type, const std::string& name, uint32 flags=0);
bool gpp_ground_unregister(uint32 type);
bool gpp_ground_get(uint32 type, gpp_ground* g);
}
#endif

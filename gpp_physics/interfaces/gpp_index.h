
/**
*Classe de index para identificar paredes, zonas, objetos, npcs, etc.... e etc....
*esta classe armazena o vnum do item e seu subvnum..
*Um vnum é um número exclusivo que o objeto terá. Este número pode variar entre
*0 e gpp_max_vnum, o que atualmente é 4000000000. Evite usar o vnum 0. Ele será conciderado como inválido.
*Dois objetos do mesmo tipo não poderão ter o mesmo vnum. Mas dois objetos diferentes poderão ter o mesmo vnum.
*Por exemplo, duas paredes não poderão ter o vnum 1, mas, uma parede e uma zona poderão ter vnum 1.
*O subvnum é usado caso precise de um vnum extra em um objeto.
*Por exemplo, uma parede com vnum 1, pode ter várias cópias de si mesmo. Então cada cópia terá um subvnum único.
*O método generate_index é responsável por gerar um subvnum único e exclusivo para o vnum principal. É recomendável não usar este método diretamente, nem alterar o subvnum.
**/

#ifndef GPP_INDEX_H
#define GPP_INDEX_H

#include<string>
#include<initializer_list>
namespace gpp
{
#define GPP_INVALID_VNUM 0//Se algum objeto tiver este vnum, ele será considerado como inválido.
#define GPP_MAX_VNUM 4000000000// Se existir um objeto maior que este vnum, ele será conciderado inválido também.

class gpp_index
{
public:
uint32 vnum;//Vnum principal
uint32 subvnum;//Vnum secundário caso o objeto precise.
gpp_index();
gpp_index(uint32 vnum, uint32 subvnum);
gpp_index(const std::initializer_list<uint32>& id);
gpp_index(const gpp_index& di);
gpp_index& operator=(const std::initializer_list<uint32>& id);
gpp_index& operator=(const gpp_index& di);
bool operator<(const gpp_index& di)const;//Usado para usar esta classe como chave no objeto std::map
bool operator==(const gpp_index& di)const;
void set(uint32 vnum=GPP_INVALID_VNUM, uint32 subvnum=GPP_INVALID_VNUM);
void setVnum(uint32 vnum);
void setSubVnum(uint32 subvnum);
void setType(uint32 type);
uint32 getVnum()const;
uint32 getSubVnum()const;
std::string toString()const;
};
}
#endif

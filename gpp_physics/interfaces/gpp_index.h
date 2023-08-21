
/**
*Classe de index para identificar paredes, zonas, objetos, npcs, etc.... e etc....
*esta classe armazena o vnum do item e seu subvnum..
*Um vnum � um n�mero exclusivo que o objeto ter�. Este n�mero pode variar entre
*0 e gpp_max_vnum, o que atualmente � 4000000000. Evite usar o vnum 0. Ele ser� conciderado como inv�lido.
*Dois objetos do mesmo tipo n�o poder�o ter o mesmo vnum. Mas dois objetos diferentes poder�o ter o mesmo vnum.
*Por exemplo, duas paredes n�o poder�o ter o vnum 1, mas, uma parede e uma zona poder�o ter vnum 1.
*O subvnum � usado caso precise de um vnum extra em um objeto.
*Por exemplo, uma parede com vnum 1, pode ter v�rias c�pias de si mesmo. Ent�o cada c�pia ter� um subvnum �nico.
*O m�todo generate_index � respons�vel por gerar um subvnum �nico e exclusivo para o vnum principal. � recomend�vel n�o usar este m�todo diretamente, nem alterar o subvnum.
**/

#ifndef GPP_INDEX_H
#define GPP_INDEX_H

#include<string>
#include<initializer_list>
namespace gpp
{
#define GPP_INVALID_VNUM 0//Se algum objeto tiver este vnum, ele ser� considerado como inv�lido.
#define GPP_MAX_VNUM 4000000000// Se existir um objeto maior que este vnum, ele ser� conciderado inv�lido tamb�m.

class gpp_index
{
public:
uint32 vnum;//Vnum principal
uint32 subvnum;//Vnum secund�rio caso o objeto precise.
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

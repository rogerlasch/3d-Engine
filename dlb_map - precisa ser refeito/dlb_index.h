
/**
*Classe de index para identificar paredes, zonas, objetos, npcs, etc.... e etc....
*esta classe armazena o vnum do item e seu subvnum..
*Um vnum � um n�mero exclusivo que o objeto ter�. Este n�mero pode variar entre
*0 e dlb_max_vnum, o que atualmente � 4000000000. Evite usar o vnum 0. Ele ser� conciderado como inv�lido.
*Dois objetos do mesmo tipo n�o poder�o ter o mesmo vnum. Mas dois objetos diferentes poder�o ter o mesmo vnum.
*Por exemplo, duas paredes n�o poder�o ter o vnum 1, mas, uma parede e uma zona poder�o ter vnum 1.
*O subvnum � usado caso precise de um vnum extra em um objeto.
*Por exemplo, uma parede com vnum 1, pode ter v�rias c�pias de si mesmo. Ent�o cada c�pia ter� um subvnum �nico.
*O m�todo generate_index � respons�vel por gerar um subvnum �nico e exclusivo para o vnum principal. � recomend�vel n�o usar este m�todo diretamente, nem alterar o subvnum.
**/

#ifndef DLB_INDEX_H
#define DLB_INDEX_H

namespace dlb
{
#define dlb_invalid_vnum 0//Se algum objeto tiver este vnum, ele ser� considerado como inv�lido.
#define dlb_max_vnum 4000000000// Se existir um objeto maior que este vnum, ele ser� conciderado inv�lido tamb�m.

class dlb_index
{
public:
uint32 vnum;//Vnum principal
uint32 subvnum;//Vnum secund�rio caso o objeto precise.
dlb_index(uint32 vnum=dlb_invalid_vnum, uint32 subvnum=dlb_invalid_vnum);
dlb_index(const dlb_index& di);
dlb_index& operator=(const dlb_index& di);
bool operator<(const dlb_index& di)const;//Usado para usar esta classe como chave no objeto std::map
bool operator==(const dlb_index& di)const;
void set(uint32 vnum=dlb_invalid_vnum, uint32 subvnum=dlb_invalid_vnum);
void setvnum(uint32 vnum);
void set_subvnum(uint32 subvnum);
uint32 getvnum()const;
uint32 get_subvnum()const;
};
}
#endif

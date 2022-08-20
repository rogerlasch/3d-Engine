
/**
*Classe de index para identificar paredes, zonas, objetos, npcs, etc.... e etc....
*esta classe armazena o vnum do item e seu subvnum..
*Um vnum é um número exclusivo que o objeto terá. Este número pode variar entre
*0 e dlb_max_vnum, o que atualmente é 4000000000. Evite usar o vnum 0. Ele será conciderado como inválido.
*Dois objetos do mesmo tipo não poderão ter o mesmo vnum. Mas dois objetos diferentes poderão ter o mesmo vnum.
*Por exemplo, duas paredes não poderão ter o vnum 1, mas, uma parede e uma zona poderão ter vnum 1.
*O subvnum é usado caso precise de um vnum extra em um objeto.
*Por exemplo, uma parede com vnum 1, pode ter várias cópias de si mesmo. Então cada cópia terá um subvnum único.
*O método generate_index é responsável por gerar um subvnum único e exclusivo para o vnum principal. É recomendável não usar este método diretamente, nem alterar o subvnum.
**/

#ifndef DLB_INDEX_H
#define DLB_INDEX_H

namespace dlb
{
#define dlb_invalid_vnum 0//Se algum objeto tiver este vnum, ele será considerado como inválido.
#define dlb_max_vnum 4000000000// Se existir um objeto maior que este vnum, ele será conciderado inválido também.

class dlb_index
{
public:
uint32 vnum;//Vnum principal
uint32 subvnum;//Vnum secundário caso o objeto precise.
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

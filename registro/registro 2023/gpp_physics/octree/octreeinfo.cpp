


#include<sstream>
#include<map>
#include"../types.h"
#include"octreeinfo.h"

using namespace std;

namespace gpp{

octreeinfo::octreeinfo()
{
this->max_depth=0;
this->blimit=0;
this->nodes=0;
blevels.clear();
}

octreeinfo::octreeinfo(const octreeinfo& info)
{
*this=info;
}

octreeinfo& octreeinfo::operator=(const octreeinfo& info)
{
this->blimit=info.blimit;
this->max_depth=info.max_depth;
this->nodes=info.nodes;
this->blevels=info.blevels;
return *this;
}

octreeinfo::~octreeinfo()
{
}

string octreeinfo::toString()const
{
stringstream ss;
ss<<fixed;
ss<<"Imprimindo árvore octree..."<<endl;
ss<<"Número de nós: "<<nodes<<endl;
ss<<"Número de objetos: "<<nbodies<<endl;
ss<<"Profundidade máxima: "<<max_depth<<endl;
ss<<"Limite de objetos por nó: "<<blimit<<endl;
ss<<"Listando quantidade de objetos por nível:"<<endl;
ss<<"Nível\tObjetos"<<endl;
for(auto& it : blevels)
{
ss<<it.first<<"\t"<<it.second<<endl;
}
return ss.str();
}

void octreeinfo::cleanup()
{
this->max_depth=0;
this->blimit=0;
this->nodes=0;
blevels.clear();
}
}

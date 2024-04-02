


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
return *this;
}

octreeinfo::~octreeinfo()
{
}

string octreeinfo::toString()const
{
stringstream ss;
ss<<fixed;
ss<<"Imprimindo �rvore octree..."<<endl;
ss<<"N�mero de n�s: "<<nodes<<endl;
ss<<"N�mero de objetos: "<<nbodies<<endl;
ss<<"Profundidade m�xima: "<<max_depth<<endl;
ss<<"Limite de objetos por n�: "<<blimit<<endl;
ss<<"Listando quantidade de objetos por n�vel:"<<endl;
ss<<"N�vel\tObjetos"<<endl;
return ss.str();
}

void octreeinfo::cleanup()
{
this->max_depth=0;
this->blimit=0;
this->nodes=0;
}
}

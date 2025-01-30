

#include"net.h"
#include"enet/EnetBase.h"

namespace gpp{
NetBase* createEnetBase(){
return new EnetBase();
}
}

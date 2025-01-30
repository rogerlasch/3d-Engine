


#ifndef G_START_H
#define G_START_H

#include<sstream>
#define F_NAME "main.txt"

namespace gpp{
struct G_STARTER{
G_STARTER(){
setlocale(LC_ALL, "Portuguese");
logger::open(new LogFile(F_NAME), LG_ALL);
std::cout<<std::fixed;
std::cout.precision(2);
}
~G_STARTER(){
}
};
}
#endif

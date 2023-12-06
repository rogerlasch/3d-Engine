


#ifndef G_START_H
#define G_START_H

#define F_NAME "main.txt"
namespace gpp{
struct G_STARTER{
G_STARTER(){
setlocale(LC_ALL, "Portuguese");
profiler_start();
log_create(F_NAME, LG_ALL);
log_set_default(F_NAME);
std::cout<<std::fixed;
std::cout.precision(2);
}
~G_STARTER(){
profiler_dump("main_profiler.txt");
}
};

}
#endif

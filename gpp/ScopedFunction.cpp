

#include"types.h"
#include"debug.h"
#include"time.h"
#include"ScopedFunction.h"

using namespace std;

namespace gpp
{
ScopedFunction::ScopedFunction(const string& func_name)
{
this->func_name=func_name;
if(func_name.size()>0)
{
start=get_timestamp_ms();
_FLOG("Entrando na função: {}", func_name);
}
}

ScopedFunction::~ScopedFunction()
{
int64 end=get_timestamp_ms();
if(func_name.size()>0)
{
_FLOG("{} encerrada depois de {} ms", func_name, (end-start));
}
}
}

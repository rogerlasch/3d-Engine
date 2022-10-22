

#include<atomic>
#include<functional>
#include<shared_mutex>
#include<mutex>
#include"types.h"
#include"debug.h"
#include"SafeFlags.h"
#include"command.h"

namespace gpp
{

command::command():
SafeFlags(0)
{
this->name="";
this->hfunc=NULL;
}

bool command::isValid()const
{
return name.size()>0&&hfunc!=NULL;
}
}

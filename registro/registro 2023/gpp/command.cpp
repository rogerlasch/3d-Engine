

#include<atomic>
#include<functional>
#include<shared_mutex>
#include<mutex>
#include"debug_system.h"
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

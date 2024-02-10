
#include<fstream>
#include<iostream>
#include<string>
#include<sstream>
#include<memory>
#include<mutex>
#include<unordered_map>
#include"../types.h"
#include"../gpp_calendar.h"
#include"logger.h"
#include"log.h"

using namespace std;

namespace gpp
{

typedef shared_ptr<logger> shared_logger;
unordered_map<string, shared_logger> logs;
shared_logger default_log;

mutex mtx_log;

static shared_logger get_log(const string& filename)
{
lock_guard<mutex> lck(mtx_log);
auto it=logs.find(filename);
return ((it==logs.end()) ? shared_logger() : it->second);
}

bool log_create(const string& filename, int32 level)
{
try {
lock_guard<mutex> lck(mtx_log);
if((filename.size()==0)||(logs.find(filename)!=logs.end()))
{
return false;
}
shared_logger lg=make_shared<logger>(filename, level);
logs.insert(make_pair(filename, lg));
gpp_calendar c;
} catch(const exception& e) {
return false;
}
return true;
}

void log_close(const string& filename)
{
lock_guard<mutex> lck(mtx_log);
auto it=logs.find(filename);
if(it!=logs.end())
{
logs.erase(it);
}
}

bool log_set_level(const string& filename, int32 level)
{
shared_logger log=get_log(filename);
if(log!=NULL)
{
log->level=level;
return true;
}
return false;
}

bool log_setopt(const string& filename, uint32 opt, bool enable)
{
shared_logger lg=get_log(filename);
if(lg==NULL)
{
return false;
}
if(enable)
{
if((lg->flags&opt)==0)
{
lg->flags|=opt;
return true;
}
}
else
{
if((lg->flags&opt)==opt)
{
lg->flags^=opt;
return true;
}
}
return true;
}

bool log_getopt(const string& filename, uint32 opt)
{
shared_logger lg=get_log(filename);
if(lg==NULL)
{
return false;
}
return (lg->flags&opt)==opt;
}

bool  log_set_default(const string& filename)
{
if(filename.size()==0)
{
default_log=shared_logger();
return true;
}
shared_logger lg=get_log(filename);
if(lg==NULL)
{
return false;
}
default_log=lg;
return true;
}

void log_write(const string& filename, int32 level, const string& str)
{
static unordered_map<int, string> lg_table={
{LG_ALL, ""},
{LG_DEBUG, "Debug"},
{LG_BACKTRACE, "BackTrace"},
{LG_ERROR, "Error"},
{LG_WARNING, "Warning"},
//{LG_INFO, "Info"},
{LG_INFO, ""},
{LG_CRITICAL, "Falha Crítica"}
};
shared_logger lg=((filename.size()>0) ? get_log(filename) : default_log);
if(lg!=NULL)
{
if(level<lg->level)
{
return;
}
string tname=lg_table.at(level);
//if(tname.size()==0) tname="Unknown";
string final=safe_format("{} {}", tname, str);
switch(level)
{
case LG_BACKTRACE:
{
lg->stacktrace.emplace_back(final);
break;
}
default:
{
lg->ofn<<final<<endl;
if((lg->flags&GLOG_CONSOLE)==GLOG_CONSOLE)
{
cout<<final<<endl;
}
break;
}
}
}
}

void log_start_backtrace(const string& filename)
{
shared_logger log=get_log(filename);
if(log!=NULL)
{
log->stacktrace.clear();
log->stacktrace.emplace_back("Starting BackTrace...");
}
}

void log_dump_backtrace(const string& filename)
{
shared_logger log=get_log(filename);
if(log!=NULL)
{
if(log->stacktrace.size()>1)
{
log->stacktrace.emplace_back("End BackTrace...");
for(auto& row : log->stacktrace)
{
log->ofn<<row<<endl;
}
log->stacktrace.resize(0);
}
}
}
}

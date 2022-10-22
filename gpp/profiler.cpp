

#include<atomic>
#include<fstream>
#include<mutex>
#include<unordered_map>
#include<sstream>
#include"types.h"
#include"time.h"
#include"profiler.h"

using namespace std;
namespace gpp
{

struct ProfilerData
{
string func_name;
atomic<int64> total_time;
atomic<int64> executions;
ProfilerData()
{
func_name="";
total_time.store(0);
executions.store(0);
}
ProfilerData(const ProfilerData& pf)
{
*this=pf;
}
ProfilerData& operator=(const ProfilerData& pf)
{
this->func_name=pf.func_name;
this->total_time.store(pf.total_time.load());
this->executions.store(pf.executions.load());
return *this;
}
string to_string()
{
stringstream ss;
int64 average_time=0;
if(executions.load()>1)
{
average_time=(total_time.load()/executions.load());
}
ss<<func_name<<": Tempo total: "<<total_time.load()<<"ms, Número de execuções: "<<executions.load();
if(average_time>0)
{
ss<<" tempo médio de execução: "<<average_time<<" ms";
}
ss<<endl;
}
};

int64 start_time=0;
unordered_map<string, ProfilerData> profiler;

void profiler_start()
{
profiler_reset();
}

void profiler_reset()
{
profiler.clear();
start_time=get_timestamp_ms();
}

void profiler_log(const string& func_name, int64 ms)
{
auto it=profiler.find(func_name);
if(it==profiler.end())
{
ProfilerData pf;
pf.func_name=func_name;
pf.total_time.fetch_add(ms);
pf.executions.fetch_add(1);
profiler.insert(make_pair(pf.func_name, pf));
}
else
{
it->second.total_time.fetch_add(ms);
it->second.executions.fetch_add(1);
}
}

void profiler_dump(const string& filename)
{
if(filename.size()==0)
{
return;
}
ofstream ofn(filename);
if(!ofn.is_open())
{
return;
}
int64 final=get_timestamp_ms()-start_time;
ofn<<"Tempo total de execução: "<<final<<" ms."<<endl;
ofn<<"Total de funções monitoradas: "<<profiler.size()<<endl;
for(auto it=profiler.begin(); it!=profiler.end(); ++it)
{
ofn<<it->first<<": Tempo total: "<<it->second.total_time.load()<<", Total de invocações: "<<it->second.executions.load()<<endl;
}
ofn.close();
}

ProfilerSnap::ProfilerSnap(const string& func_name)
{
this->func_name=func_name;
start=get_timestamp_ms();
}

ProfilerSnap::~ProfilerSnap()
{
int64 end=get_timestamp_ms();
profiler_log(func_name, (end-start));
}

}

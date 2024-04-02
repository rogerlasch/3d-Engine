

#include<sstream>
#include<fstream>
#include<string>
#include<unordered_map>
#include"../types.h"
#include"FunctionInfo.h"
#include"profiler.h"

using namespace std;

namespace gpp
{

int64 start_time=0;
unordered_map<string, FunctionInfo*> fn_table;


void profiler_start()
{
if(start_time==0)
{
start_time=get_timestamp_ms();
for(auto& it : fn_table){
delete it.second;
}
fn_table.clear();
}
}

void profiler_dump(const string& filename, uint32 tType)
{
stringstream ss;
if(start_time==0)
{
return;
}
string fname=((filename.size()==0) ? "gpp_profiler.txt" : filename);
//ss<<"Tempo de execução total: "<<profiler_format_time(get_timestamp_ms()-start_time, htype)<<endl;
ss<<"Nome, Tempo médio, Execuções, Menor Tempo, Maior Tempo, Unidade"<<endl;
for(auto it=fn_table.begin(); it!=fn_table.end(); ++it)
{
ss<<it->second->dump(tType);
delete it->second;
}
fn_table.clear();

ofstream ofn(fname);
ofn<<ss.str();
ofn.close();
}

void profiler_log_snap(const string& func_name, int64 execution_time_ns)
{
auto it=fn_table.find(func_name);
if(it==fn_table.end())
{
FunctionInfo* fn=new FunctionInfo();
fn->name=func_name;
fn->executions=1;
fn->total_time=execution_time_ns;
fn->lowTime=execution_time_ns;
fn->highTime=execution_time_ns;
fn_table.insert(make_pair(func_name, fn));
}
else
{
    it->second->executions += 1;
    it->second->total_time += execution_time_ns;
it->second->lowTime=std::min(it->second->lowTime, execution_time_ns);
it->second->highTime=std::max(it->second->highTime, execution_time_ns);
}
}

decimal profiler_format_time(int64 ns, uint32 tType) {
    switch(tType) {
        case PFD_NANOSECONDS:
            return static_cast<decimal>(ns);
        case PFD_MICROSECONDS:
            return static_cast<decimal>(ns) / 1000.0;
        case PFD_MILLISECONDS:
            return static_cast<decimal>(ns) / 1000000.0;
        default:
            return -1.0;
    }
}

string profiler_get_time_desk(uint32 tType){
    switch(tType) {
        case PFD_NANOSECONDS:
return "Nanosegundos";
        case PFD_MICROSECONDS:
            return "Microsegundos";
        case PFD_MILLISECONDS:
            return "Milisegundos";
        default:
            return "Desconhecido";
    }
}
}

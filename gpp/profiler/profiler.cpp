

#include<sstream>
#include<fstream>
#include<string>
#include<unordered_map>
#include"../types.h"
#include"../gpp_value.h"
#include"FunctionInfo.h"
#include"ProfilerTable.h"
#include"profiler.h"

using namespace std;

namespace gpp
{

int64 start_time=0;
unordered_map<string, functioninfo_ptr> fn_table;
unordered_map<string, int64> counter_table;
unordered_map<string, gpp_value> v_table;
unordered_map<string, profiler_table_ptr> htables;

void profiler_start()
{
if(start_time==0)
{
start_time=get_timestamp_ms();
fn_table.clear();
counter_table.clear();
v_table.clear();
htables.clear();
}
}

void profiler_dump(const string& filename)
{
stringstream ss;
if(start_time==0)
{
return;
}
string fname=((filename.size()==0) ? "gpp_profiler.txt" : filename);
ss<<"Tempo de execução total: "<<(get_timestamp_ms()-start_time)<<" ms"<<endl;
ss<<endl<<"Imprimindo funções monitoradas:"<<endl;
ss<<"Total de funções monitoradas: "<<fn_table.size()<<endl<<endl;
for(auto it=fn_table.begin(); it!=fn_table.end(); ++it)
{
ss<<it->second->toString();
}
if(counter_table.size()>0)
{
ss<<endl<<"Imprimindo variáveis de contagem..."<<endl;
ss<<"Total de variáveis a imprimir: "<<counter_table.size()<<endl<<endl;
for(auto it=counter_table.begin(); it!=counter_table.end(); ++it)
{
ss<<it->first<<": "<<it->second<<endl;
}
}
if(v_table.size()>0)
{
ss<<endl<<"Imprimindo variáveis gerais..."<<endl;
ss<<"Total de variáveis a imprimir: "<<v_table.size()<<endl<<endl;
for(auto it=v_table.begin(); it!=v_table.end(); ++it)
{
ss<<it->first<<": "<<it->second.toString()<<endl;
}
}
if(htables.size()>0)
{
ss<<"Imprimindo tabelas..."<<endl;
ss<<"Total de tabelas a imprimir: "<<htables.size()<<endl<<endl;
for(auto it=htables.begin(); it!=htables.end(); ++it)
{
ss<<it->second->toString()<<endl;
}
}
ofstream ofn(fname);
ofn<<fixed;
ofn.precision(2);
ofn<<ss.str();
ofn.close();
}

void profiler_log_snap(const string& func_name, int64 execution_time_ns)
{
auto it=fn_table.find(func_name);
if(it==fn_table.end())
{
functioninfo_ptr fn=make_shared<FunctionInfo>();
fn->func_name=func_name;
fn->total_time.fetch_add(execution_time_ns);
fn->total_executions.fetch_add(1);
fn_table.insert(make_pair(func_name, fn));
}
else
{
it->second->total_time.fetch_add(execution_time_ns);
it->second->total_executions.fetch_add(1);
}
}

void profiler_set_int(const string& vname, int64 value)
{
auto it=counter_table.find(vname);
if(it==counter_table.end())
{
counter_table.insert(make_pair(vname, value));
}
else
{
it->second=value;
}
}

int64 profiler_get_int(const string& vname)
{
auto it=counter_table.find(vname);
return ((it==counter_table.end()) ? 0 : it->second);
}

void profiler_inc(const string& vname, int64 qtd)
{
auto it=counter_table.find(vname);
if(it==counter_table.end())
{
profiler_set_int(vname, qtd);
}
else
{
it->second+=qtd;
}
}

void profiler_dec(const string& vname, int64 qtd)
{
auto it=counter_table.find(vname);
if(it==counter_table.end())
{
profiler_set_int(vname, qtd);
}
else
{
it->second-=qtd;
}
}

void profiler_set_value(const string& var_name, const gpp_value& val)
{
auto it=v_table.find(var_name);
if(it==v_table.end())
{
v_table.insert(make_pair(var_name, val));
}
else
{
it->second=val;
}
}

gpp_value profiler_get_value(const string& var_name)
{
auto it=v_table.find(var_name);
return ((it==v_table.end()) ? gpp_value() : it->second);
}

void profiler_create_table(const string& tname, const vector<string> header)
{
auto it=htables.find(tname);
if((it==htables.end())&&(tname.size()>0)&&(header.size()>0))
{
shared_ptr<ProfilerTable> hs=make_shared<ProfilerTable>();
hs->appendHeader(header);
htables.insert(make_pair(tname, hs));
}
}

void profiler_add_row(const string& tname, const vector<gpp_value>& values)
{
auto it=htables.find(tname);
if(it!=htables.end())
{
it->second->addRow(values);
}
}
}

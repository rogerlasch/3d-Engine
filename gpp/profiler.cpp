

#include<atomic>
#include<iostream>
#include<fstream>
#include<mutex>
#include<unordered_map>
#include<sstream>
#include"types.h"
#include"debug.h"
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
string toString()
{
stringstream ss;
ss<<fixed;
ss.precision(2);
float ftime=static_cast<float>(total_time.load());
ftime/=1000000;
float average_time=0.0f;
if(executions.load()>1)
{
average_time=(ftime/(float)executions.load());
}
ss<<func_name<<": Tempo total: "<<ftime<<" ms, Total de execuções: "<<executions.load();
if(average_time!=0)
{
ss<<", Duração média: "<<average_time<<" ms"<<endl;
}
else
{
ss<<endl;
}
return ss.str();
}
};

struct matrix_data
{
uint32 cols;
string key_name;
vector<string> lines;
matrix_data()
{
key_name="";
cols=0;
lines.resize(0);
}
void addHeader(const vector<string>& col_names)
{
cols=col_names.size();
stringstream ss;
for(auto& it : col_names)
{
ss<<it<<" ";
}
ss<<endl;
lines.push_back(ss.str());
}
void appendRow(const gpp_value_args& args)
{
uint32 x=0;
stringstream ss;
for(auto& it : args)
{
x++;
ss<<it.toString()<<" ";
if(x>=cols)
{
break;
}
}
if(args.size()<cols)
{
for(uint32 i=args.size(); i<cols; i++)
{
ss<<"NULL ";
}
}
lines.push_back(ss.str());
}
string toString()const
{
stringstream ss;
for(auto& it : lines)
{
ss<<it<<endl;
}
return ss.str();
}
};

int64 start_time=0;
unordered_map<string, ProfilerData> profiler;
unordered_map<string, atomic<int64>> var_data;
unordered_map<string, shared_ptr<matrix_data>> tbl_matrix;


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

void profiler_set_int(const string& var_name, int64 value)
{
auto it=var_data.find(var_name);
if(it==var_data.end())
{
var_data.insert(make_pair(var_name, value));
}
else
{
it->second.store(value);
}
}

int64 profiler_get_int(const string& var_name)
{
auto it=var_data.find(var_name);
return ((it==var_data.end()) ? 0 : it->second.load());
}

void profiler_inc(const string& var_name, int64 qtd)
{
auto it=var_data.find(var_name);
if(it==var_data.end())
{
profiler_set_int(var_name, qtd);
}
else
{
it->second.fetch_add(qtd);
}
}

void profiler_dec(const string& var_name, int64 qtd)
{
auto it=var_data.find(var_name);
if(it==var_data.end())
{
profiler_set_int(var_name, qtd);
}
else
{
it->second.fetch_sub(qtd);
}
}

void profiler_create_matrix(const string& key_name, const vector<string>& col_names)
{
if(tbl_matrix.find(key_name)==tbl_matrix.end())
{
shared_ptr<matrix_data> mt=make_shared<matrix_data>();
mt->key_name=key_name;
mt->addHeader(col_names);
tbl_matrix.insert(make_pair(key_name, mt));
}
}

void profiler_add_row(const string& key_name, const gpp_value_args& args)
{
auto it=tbl_matrix.find(key_name);
if(it!=tbl_matrix.end())
{
it->second->appendRow(args);
}
}

void profiler_dump(const string& filename)
{
if(filename.size()==0)
{
return;
}
stringstream ss;
ofstream ofn(filename);
if(!ofn.is_open())
{
return;
}
int64 final=get_timestamp_ms()-start_time;
ss<<"Tempo total de execução: "<<final<<" ms."<<endl;
ss<<"Total de funções monitoradas: "<<profiler.size()<<endl;
for(auto it=profiler.begin(); it!=profiler.end(); ++it)
{
ss<<it->second.toString();
}
if(var_data.size()>0)
{
ss<<"Imprimindo variáveis contadoras..."<<endl;
for(auto it=var_data.begin(); it!=var_data.end(); ++it)
{
ss<<it->first<<": "<<it->second.load()<<endl;
}
}
if(tbl_matrix.size()>0)
{
ss<<"Imprimindo matrizes... Total de matrizes a imprimir: "<<tbl_matrix.size()<<endl<<endl;
for(auto it=tbl_matrix.begin(); it!=tbl_matrix.end(); ++it)
{
ss<<"Imprimindo "<<it->first<<endl;
ss<<it->second->toString()<<endl;
}
}
ofn<<ss.str();
ofn.close();
}

ProfilerSnap::ProfilerSnap(const string& func_name)
{
this->func_name=func_name;
start=get_timestamp_ns();
}

ProfilerSnap::~ProfilerSnap()
{
int64 end=get_timestamp_ns();
profiler_log(func_name, (end-start));
}

}

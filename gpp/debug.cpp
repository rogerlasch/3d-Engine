


#include<fstream>
#include<iostream>
#include<mutex>
#include<string>
#include"types.h"
#include"debug.h"

using namespace std;

namespace gpp
{

static bool mstdout=true;
static ofstream log_file;
static mutex mtx_log;

void log_set_stdout(bool toggle_stdout)
{
mstdout=toggle_stdout;
}

void log_open_file(const string& filename)
{
try {
lock_guard<mutex> lck(mtx_log);
if((!log_file.is_open())&&(filename.size()>0))
{
log_file.open(filename);
}
} catch(const exception& e) {
cout<<"Exception: "<<e.what()<<endl;
}
}

bool log_is_open()
{
lock_guard<mutex> lck(mtx_log);
return log_file.is_open();
}

void log_write(uint32 type, const string& msg)
{
if(!log_is_open())
{
log_open_file("gpp_default_file.txt");
}
lock_guard<mutex> lck(mtx_log);
switch(type)
{
case LOG_MSG:
case LOG_FAILURE:
{
log_file<<msg<<endl;
if(mstdout)
{
cout<<msg<<endl;
}
break;
}
}
}

void log_write_except(const string& filename, const string& func_name, const string& edesc)
{
static string except_format="Exceção gerada na função {} no arquivo {}.\nMensagem de erro: \"{}\"\n";
string str=safe_format(except_format, filename, func_name, edesc);
log_write(LOG_FAILURE, str);
}

void log_write_assert(const string& filename, const string& func_name, uint32 line, const string expression, const string& msg)
{
string final="";
if(msg.size()==0)
{
string assert_format="Erro de assertion no arquivo {}\nFunção: {} Linha: {}.\nExpressão avaliada: {}\n";
final=safe_format(assert_format, filename, func_name, line, expression);
}
else
{
string assert_format="Erro de assertion no arquivo {}\nFunção: {} Linha: {}.\nExpressão avaliada: {}\nMensagem de erro: \"{}\"";
final=safe_format(assert_format, filename, func_name, line, expression, msg);
}
log_write(LOG_FAILURE, final);
std::terminate();
}
}

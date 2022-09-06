


#include<fstream>
#include<iostream>
#include<mutex>
#include<string>
#include"types.h"
#include"debug.h"

using namespace std;

namespace gpp
{

void log_write(uint32 type, const string& msg)
{
static ofstream ofn("gpp_log.txt");
static mutex mtx;
unique_lock<mutex> lck(mtx);
switch(type)
{
case LOG_MSG:
case LOG_FAILURE:
{
ofn<<msg<<endl;
cout<<msg<<endl;
}
}
}

void log_write_except(const string& filename, const string& func_name, const string& edesc)
{
static string except_format="Exceção gerada na função {} no arquivo {}.\nMensagem de erro: \"{}\"\n";
string str=safe_format(except_format, filename, func_name, edesc);
log_write(LOG_FAILURE, str);
}

void log_write_assert(const string& filename, const string& func_name, uint32 line, const string expression)
{
static string assert_format="Erro de assertion no arquivo {}\nFunção: {} Linha: {}.\nExpressão avaliada: {}.\n";
string str=safe_format(assert_format, func_name, filename, line, expression);
log_write(LOG_FAILURE, str);
std::terminate();
}
}

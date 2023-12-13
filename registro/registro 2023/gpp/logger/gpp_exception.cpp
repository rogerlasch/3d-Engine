

#include<sstream>
#include"gpp_exception.h"

using namespace std;

namespace gpp
{
gpp_exception::gpp_exception(const string& msg)
{
this->msg=msg;
}

gpp_exception::gpp_exception(const string& msg, const string& function_name, const string& filename, int line)
{
stringstream ss;
ss<<"Excessão gerada na função "<<function_name<<". Arquivo: "<<filename<<", Linha: "<<line<<endl<<"Descrição: "<<msg<<endl;
this->msg=ss.str();
}

gpp_exception::gpp_exception(const gpp_exception& gx)
{
*this=gx;
}

gpp_exception& gpp_exception::operator=(const gpp_exception& gx)
{
this->msg=gx.msg;
return *this;
}

gpp_exception::~gpp_exception()
{
}

const char * gpp_exception::what ( ) const throw ( )
{
return msg.c_str();
}
}


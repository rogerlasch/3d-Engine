

#include<string>
#include<sstream>
#include<vector>
#include"types.h"
#include"cmd_parser.h"

using namespace std;

namespace gpp
{
cmd_parser::cmd_parser(const string& str, uint32 required_args)
{
this->parse_string(str, required_args);
}

cmd_parser::cmd_parser(const cmd_parser& ps)
{
*this=ps;
}

cmd_parser& cmd_parser::operator=(const cmd_parser& ps)
{
this->str=ps.str;
this->args=ps.args;
return *this;
}

string cmd_parser::operator[](uint32 index)const
{
if(index>=args.size())
{
return "";
}
return args[index];
}

string cmd_parser::get_args(uint32 start_index, uint32 end_index)
{
if(start_index>end_index)
{
uint32 x=start_index;
start_index=end_index;
end_index=x;
}
if(start_index>=args.size())
{
return "";
}
if(end_index>args.size())
{
end_index=args.size();
}
if(start_index==end_index)
{
return args[start_index];
}
stringstream ss;
for(uint32 i=start_index; i<end_index; i++)
{
ss<<args[i];
if(i<(end_index-1))
{
ss<<' ';
}
}
return ss.str();
}

void cmd_parser::cleanup()
{
str.resize(0);
args.resize(0);
}

uint32 cmd_parser::parse_string(const string& str, uint32 required_args)
{
this->cleanup();
this->str=str;
string sline="";
sline.reserve(1024);
bool apostrofe_open=false;
for(uint32 i=0; i<=str.size(); i++)
{
if(i==str.size())
{
if(sline.size()>0)
{
if((apostrofe_open)&&(sline[sline.size()-1]!='\''))
{
sline+='\'';
}
args.push_back(sline);
}
break;
}
if((required_args>0)&&(args.size()>=required_args-1))
{
string ls=str.substr(i, str.size());
this->trim(ls);
args.push_back(ls);
break;
}
switch(str[i])
{
case ' ':
{
//Pule os espaços...
if(sline.size()==0)
{
continue;
}
if(apostrofe_open)
{
if((i>0)&&(str[i-1]!=' '))
{
sline+=' ';
continue;
}
continue;
}
//caso contrário, adicione o argumento na lista...
args.push_back(sline);
sline.resize(0);
break;
}
case '\'':
case '\"':
{
apostrofe_open=((apostrofe_open==true) ? false : true);
sline+='\'';
if(apostrofe_open==false)
{
args.push_back(sline);
sline.resize(0);
}
break;
}
default:
{
sline+=str[i];
break;
}
}
}
return args.size();
}

uint32 cmd_parser::size()const
{
return args.size();
}

bool cmd_parser::exists(const string& arg, uint32* out_index)const
{
uint32 i=0;
for(auto it=args.begin(); it!=args.end(); ++it)
{
if((*it)==arg)
{
if(out_index!=NULL)
{
*out_index=i;
}
return true;
}
i++;
}
return false;
}

void cmd_parser::trim(string& s, const string& t)
{
string::size_type i=s.find_last_not_of(t);
if(i==string::npos)
{
return;
}
s.erase(i+1).erase(0, s.find_first_not_of(t));
}
}

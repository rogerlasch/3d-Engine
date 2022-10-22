

#include<string>
#include"types.h"
#include"string_utils.h"

using namespace std;

namespace gpp
{
string string_to_lower_case(const string& s)
{
string final="";
final.reserve(s.size());
for(uint32 i=0; i<s.size(); i++)
{
final+=tolower(s[i]);
}
return final;
}

void string_to_lower_case(string& s)
{
for(uint32 i=0; i<s.size(); i++)
{
s[i]=tolower(s[i]);
}
}

string string_to_upper_case(const string& s)
{
string final="";
final.reserve(s.size());
for(uint32 i=0; i<s.size(); i++)
{
final+=toupper(s[i]);
}
return final;
}

void string_to_upper_case(string& s)
{
for(uint32 i=0; i<s.size(); i++)
{
s[i]=toupper(s[i]);
}
}

string string_trim(const string& s)
{
static string t = " \t\r\n";
string d = s;
string::size_type i = d.find_last_not_of(t);
if(i ==std::string::npos)
{
         return "";
}
      else
{
         return d.erase(i + 1).erase(0, s.find_first_not_of(t));
}
}

void string_trim(string& s)
{
static string t = " \t\r\n";
string::size_type i=s.find_last_not_of(t);
if(i ==std::string::npos)
{
return;
}
      else
{
s.erase(i + 1).erase(0, s.find_first_not_of(t));
}
}
}

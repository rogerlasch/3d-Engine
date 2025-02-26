
//#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#include<codecvt>
#include<string>
#include"types.h"
#include"logger/gpp_exception.h"
#include"logger/logger.h"
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

wstring wstring_to_lower_case(const wstring& s)
{
wstring final=L"";
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

void wstring_to_lower_case(wstring& s)
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

wstring wstring_to_upper_case(const wstring& s)
{
wstring final=L"";
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

void wstring_to_upper_case(wstring& s)
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

wstring wstring_trim(const wstring& s)
{
static wstring t =L" \t\r\n";
wstring d = s;
wstring::size_type i = d.find_last_not_of(t);
if(i ==std::wstring::npos)
{
         return L"";
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

void wstring_trim(wstring& s)
{
static wstring t = L" \t\r\n";
wstring::size_type i=s.find_last_not_of(t);
if(i ==std::wstring::npos)
{
return;
}
      else
{
s.erase(i + 1).erase(0, s.find_first_not_of(t));
}
}

string string_replace(const string& str, const string& old, const string& new_str)
{
string mstr(str);
size_t x=0;
while((x=mstr.find(old, x))!=string::npos)
{
mstr.replace(x, old.size(), new_str);
}
return mstr;
}

string string_replace(const string& str, const vector<pair<string, string>>& args)
{
string mstr(str);
for(auto& it : args)
{
size_t x=0;
while((x=mstr.find(it.first, x))!=string::npos)
{
mstr.replace(x, it.first.size(), it.second);
}
}
return mstr;
}

wstring wstring_replace(const wstring& str, const wstring& old, const wstring& new_str)
{
wstring mstr(str);
size_t x=0;
while((x=mstr.find(old, x))!=wstring::npos)
{
mstr.replace(x, old.size(), new_str);
}
return mstr;
}

wstring wstring_replace(const wstring& str, const vector<pair<wstring, wstring>>& args)
{
wstring mstr(str);
for(auto& it : args)
{
size_t x=0;
while((x=mstr.find(it.first, x))!=wstring::npos)
{
mstr.replace(x, it.first.size(), it.second);
}
}
return mstr;
}

//Conversions...

string string_utf8_encode(const wstring& str)
{
try {
    wstring_convert<codecvt_utf8_utf16<wchar_t>> myconv;
    return myconv.to_bytes(str);
} catch(const exception& e) {
logger::fatal("{}, {}, {}.", __FILE__, __FUNCTION__, e.what());
}
return "";
}

wstring string_utf8_decode(const string& str)
{
try {
wstring_convert<codecvt_utf8_utf16<wchar_t>> myconv;
    return myconv.from_bytes(str);
} catch(const exception& e) {
logger::fatal("{}, {}, {}.", __FILE__, __FUNCTION__, e.what());
}
return L"";
}

string wstring_to_string(const wstring& str)
{
try{
locale loc;
const auto& _Facet=use_facet<codecvt<char, wchar_t, mbstate_t>>(loc);
string final="";
final.resize(str.size());
mbstate_t _State{};
const wchar_t* istart=str.c_str();
const wchar_t* iend=(str.c_str()+str.size());
const wchar_t* inext=NULL;
char* ostart=(char*)final.c_str();
char* oend=(char*)(final.c_str()+final.size());
char* onext=NULL;
const auto Result=_Facet.in(_State, istart, iend, inext, ostart, oend, onext);
if(Result==codecvt_base::ok)
{
final.resize(static_cast<size_t>(onext-ostart));
return final;
}
}catch(const exception& e){
return "Impossível converter de wstring para string.";
}
return "";
}

wstring string_to_wstring(const string& str)
{
locale loc;
const auto& _Facet=use_facet<codecvt<wchar_t, char, mbstate_t>>(loc);
wstring final=L"";
final.resize(str.size());
mbstate_t _State{};
const char* istart=str.c_str();
const char* iend=(str.c_str()+str.size());
const char* inext=NULL;
wchar_t* ostart=(wchar_t*)final.c_str();
wchar_t* oend=(wchar_t*)(final.c_str()+final.size());
wchar_t* onext=NULL;
const auto Result=_Facet.in(_State, istart, iend, inext, ostart, oend, onext);
if(Result==codecvt_base::ok)
{
final.resize(static_cast<size_t>(onext-ostart));
return final;
}
return L"";
}

uint32 string_split(const std::string& str, const std::string& delin, std::vector<std::string>& out) {
    out.clear();
    size_t start = 0;
    size_t end = str.find(delin);
    while (end != std::string::npos) {
        out.push_back(str.substr(start, end - start));
        start = end + delin.length();
        end = str.find(delin, start);
    }
    out.push_back(str.substr(start));
return out.size();
}

uint32 wstring_split(const std::wstring& str, const std::wstring& delin, std::vector<std::wstring>& out) {
    out.clear();
    size_t start = 0;
    size_t end = str.find(delin);
    while (end != std::wstring::npos) {
        out.push_back(str.substr(start, end - start));
        start = end + delin.length();
        end = str.find(delin, start);
    }
    out.push_back(str.substr(start));
return out.size();
}
}

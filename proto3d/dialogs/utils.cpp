
#include<regex>
#include"gui.h"

using namespace std;

bool pt3_is_numeric(const string& str) {
    // O padrão de regex verifica se a string contém apenas números, um ponto decimal opcional e um sinal negativo opcional no início.
    regex numeric_pattern("^-?\\d+(\\.\\d+)?$");
    return regex_match(str, numeric_pattern);
}

bool pt3_parseVector(const string& str, vector3d& vec)
{
string msg=string_replace(str, ",", ".");
vector<string> arr;
string_split(msg, ";", arr);
if(arr.size()==0) return false;
if(arr.size()<3)
{
arr.resize(3, "0");
}
else if(arr.size()>3)
{
arr.resize(3);
}
for(uint32 i=0; i<3; i++)
{
if(!pt3_is_numeric(arr[i])) {
vec={0.0f, 0.0f, 0.0f};
return false;
}
vec[i]=atof(arr[i].c_str());
}
return true;
}

void pt3_edit_vector(const vector3d& v, vector<wxTextCtrl*>& htext)
{
_GASSERT_MSG(htext.size()==3, "É necessário um vetor com 3 elementos.");
for(uint32 i=0; i<3; i++)
{
htext[i]->SetValue(to_string(v[i]));
}
}

bool pt3_validate(vector<wxTextCtrl*>& htext, vector3d& vec)
{
_GASSERT_MSG(htext.size()==3, "É necessário um vetor de 3 elementos");
for(uint32 i=0; i<3; i++)
{
string s=string_replace(htext[i]->GetValue().ToStdString(), ",", ".");
if((s.size()==0)||(!pt3_is_numeric(s)))
{
vec={0,0,0};
return false;
}
vec[i]=atof(s.c_str());
}
return true;
}

bool pt3_validate_string(const string& str, const string& rgx)
{
    regex r(rgx);
    return regex_match(str, r);
}

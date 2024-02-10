

#include<sstream>
#include<vector>
#include<memory>
#include<string>
#include"profiler.h"
#include"ProfilerTable.h"

using namespace std;

namespace gpp
{
ProfilerTable::ProfilerTable()
{
htable.clear();
}

void ProfilerTable::appendHeader(const vector<string>& h)
{
if((h.size()==0)||(htable.size()>1))
{
return;
}
htable.resize(1);
htable[0]=h;
}

void ProfilerTable::addRow(const vector<gpp_value>& row)
{
if(htable.size()==0)
{
return;
}
vector<string> rw;
for(auto& it : row)
{
rw.push_back(it.toString());
}
if(rw.size()>htable[0].size()) rw.resize(htable[0].size());
if(rw.size()<htable[0].size()) rw.resize(htable[0].size(), "NULL");
htable.push_back(rw);
}

string ProfilerTable::toString()
{
stringstream ss;
for(auto& it : htable)
{
for(auto& it2 : it)
{
ss<<it2<<'\t';
}
ss<<endl;
}
return ss.str();
}
}

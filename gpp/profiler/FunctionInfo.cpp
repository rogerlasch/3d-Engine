

#include<sstream>
#include"../types.h"
#include"FunctionInfo.h"
#include"profiler.h"

using namespace std;

namespace gpp
{
FunctionInfo::FunctionInfo()
{
name="";
executions=0;
total_time=0;
lowTime=0;
highTime=0;
}

FunctionInfo::~FunctionInfo(){
}

string FunctionInfo::dump(uint32 tType)const{
        std::stringstream ss;
        ss << std::fixed;
        ss.precision(2);

ss<<name<<", ";
ss<<profiler_format_time(total_time/executions, tType)<<", ";
ss<<executions<<", ";
ss<<lowTime<<", ";
ss<<highTime<<", ";
ss<<profiler_get_time_desk(tType)<<endl;
        return ss.str();
}
}

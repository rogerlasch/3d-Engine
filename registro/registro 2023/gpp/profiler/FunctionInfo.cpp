

#include<sstream>
#include"../types.h"
#include"FunctionInfo.h"

using namespace std;

namespace gpp
{
FunctionInfo::FunctionInfo()
{
func_name="";
total_executions.store(0);
total_time.store(0);
}

string FunctionInfo::toString()
{
stringstream ss;
ss<<fixed;
ss.precision(3);
float ftime=static_cast<float>(total_time.load())/1000000;
float average_time=(ftime/(float)total_executions.load());
ss<<func_name<<": Tempo total: "<<ftime<<" ms, Total de execuções: "<<total_executions.load();
if(average_time!=0.0f)
{
ss<<", Duração média: "<<average_time<<" ms"<<endl;
}
ss<<endl;
return ss.str();
}
}

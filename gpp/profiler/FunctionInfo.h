


#ifndef GPP_FUNCTIONINFO_H
#define GPP_FUNCTIONINFO_H


#include<atomic>

namespace gpp
{


class FunctionInfo
{
public:
int64 total_time;
int64 lowTime;
int64 highTime;
int64 executions;
std::string name;
FunctionInfo();
FunctionInfo(const FunctionInfo& f)=delete;
FunctionInfo& operator=(const FunctionInfo& f)=delete;
~FunctionInfo();

std::string dump(uint32 tType)const;

};

}
#endif

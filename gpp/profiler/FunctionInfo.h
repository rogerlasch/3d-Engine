


#ifndef GPP_FUNCTIONINFO_H
#define GPP_FUNCTIONINFO_H


#include<atomic>

namespace gpp
{


class FunctionInfo
{
public:
std::atomic<int64> total_time;
std::atomic<int64> total_executions;
std::string func_name;
FunctionInfo();
FunctionInfo(const FunctionInfo& f)=delete;
FunctionInfo& operator=(const FunctionInfo& f)=delete;
std::string toString();
};
typedef std::shared_ptr<FunctionInfo> functioninfo_ptr;

}
#endif

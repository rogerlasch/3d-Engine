

#ifndef GPP_SCOPEDFUNCTION_H
#define GPP_SCOPEDFUNCTION_H

namespace gpp
{

class ScopedFunction
{
private:
std::string func_name;
int64 start;
public:
ScopedFunction(const std::string& func_name);
~ScopedFunction();
};
}
#endif

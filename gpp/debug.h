


#ifndef GPP_DEBUG_H
#define GPP_DEBUG_H

#include<fmt/core.h>

#pragma warning(disable: 4834)
namespace gpp
{

enum LOG_TYPES
{
LOG_MSG=0,
LOG_FAILURE=1,
};

void log_write(uint32 type, const std::string& msg);
void log_write_except(const std::string& filename, const std::string& func_name, const std::string& edesc);
void log_write_assert(const std::string& filename, const std::string& func_name, uint32 line, const std::string expression);

template<class ...Args>
std::string safe_format(const std::string& str, const Args& ...args)
{
std::string final="";
try {
final=fmt::format(str, args...);
} catch(const std::exception& e) {
e.what();
return "";
}
return final;
}

#define _LOG_EXCEPT(edesc) log_write_except(__FILE__, __FUNCTION__, edesc)
#define _GASSERT(expr, msg) \
if(!(expr))\
log_write_assert(__FILE__, __FUNCTION__, __LINE__, #expr);
//End macro...
#define _FLOG(msg, ...) log_write(LOG_MSG, safe_format(msg, __VA_ARGS__))
}
#endif

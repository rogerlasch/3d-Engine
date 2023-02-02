


#ifndef GPP_DEBUG_H
#define GPP_DEBUG_H

#include<format>

#pragma warning(disable: 4834)
namespace gpp
{

enum LOG_TYPES
{
LOG_MSG=0,
LOG_FAILURE=1,
};

void log_set_stdout(bool toggle_stdout);
void log_open_file(const std::string& filename);
bool log_is_open();
void log_write(uint32 type, const std::string& msg);
void log_write_except(const std::string& filename, const std::string& func_name, const std::string& edesc);
void log_write_assert(const std::string& filename, const std::string& func_name, uint32 line, const std::string expression, const std::string& msg="");

template<class ...Args>
inline std::string safe_format(const std::string& str, const Args& ...args)
{
std::string final="";
try {
final=std::vformat(str, std::make_format_args(args...));
} catch(const std::exception& e) {
e.what();
return "";
}
return final;
}

template<class ...Args>
inline std::wstring safe_wformat(const std::wstring& str, const Args& ...args)
{
std::wstring final=L"";
try {
final=std::vformat(str, std::make_wformat_args(args...));
} catch(const std::exception& e) {
e.what();
return L"";
}
return final;
}

#define _LOG_EXCEPT(edesc) log_write_except(__FILE__, __FUNCTION__, edesc)
#define _GASSERT(expr) \
if(!(expr))\
log_write_assert(__FILE__, __FUNCTION__, __LINE__, #expr, "");
//End macro...
#define _GASSERT_MSG(expr, msg) \
if(!(expr))\
log_write_assert(__FILE__, __FUNCTION__, __LINE__, #expr, msg);
//End macro...

#define _FLOG(msg, ...) log_write(LOG_MSG, safe_format(msg, __VA_ARGS__))
}
#endif




#ifndef GPP_LOG_H
#define GPP_LOG_H

#pragma warning(disable: 4834)
#include<format>


namespace gpp
{

enum GPP_LOG_LEVEL
{
LG_ALL=0,
LG_DEBUG,
LG_BACKTRACE,
LG_ERROR,
LG_WARNING,
LG_INFO,
LG_CRITICAL
};


enum GPP_LOGGER_FLAGS
{
GLOG_CONSOLE=1
};

bool log_create(const std::string& filename, int32 level);
void log_close(const std::string& filename);
bool log_set_level(const std::string& filename, int32 level);
bool log_setopt(const std::string& filename, uint32 opt, bool enable);
bool log_getopt(const std::string& filename, uint32 opt);
bool log_set_default(const std::string& filename);
void log_write(const std::string& filename, int32 level, const std::string& str);
void log_start_backtrace(const std::string& filename);
void log_dump_backtrace(const std::string& filename);

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


#if defined(_DEBUG)||defined(_GPP_USE_DEBUG)
#define _GDEBUG(str, ...) log_write("", LG_DEBUG, safe_format(str, __VA_ARGS__))
#define _GBACKTRACE(str, ...) log_write("", LG_BACKTRACE, safe_format(str, __VA_ARGS__))
#define _GERROR(str, ...) log_write("", LG_ERROR, safe_format(str, __VA_ARGS__))
#define _GWARNING(str, ...) log_write("", LG_WARNING, safe_format(str, __VA_ARGS__))
#define _GINFO(str, ...) log_write("", LG_INFO, safe_format(str, __VA_ARGS__))
#define _GCRITICAL(str, ...) log_write("", LG_CRITICAL, safe_format(str, __VA_ARGS__))

#undef _GASSERT
#undef _GASSERT_MSG
#define _GASSERT(expr)\
if(!(expr)) {\
log_write("", LG_CRITICAL, safe_format("Assertion error in file: {}, Line: {}, Expression: \"{}\"", __FILE__, __LINE__, #expr));\
std::terminate();\
}
//End macro

#define _GASSERT_MSG(expr, str, ...)\
if(!(expr)) {\
log_write("", LG_CRITICAL, safe_format("Assertion error in file: {}, Line: {}, Expression: \"{}\"\nMsg: {}", __FILE__, __LINE__, #expr, safe_format(str, __VA_ARGS__)));\
std::terminate();\
}
//End macro...
#else
#define _GDEBUG(str, ...)
#define _GBACKTRACE(str, ...)
#define _GERROR(str, ...)
#define _GWARNING(str, ...)
#define _GINFO(str, ...)
#define _GCRITICAL(str, ...)

#undef _GASSERT
#undef _GASSERT_MSG

#define _GASSERT(expr)
#define _GASSERT_MSG(expr, str, ...)
#endif
}
#endif




#ifndef GPP_DEBUG_SYSTEM_H
#define GPP_DEBUG_SYSTEM_H

#include<assert.h>
#include<string>
#include"types.h"
#include"safe_format.h"
#include"time.h"
#include"logger/gpp_exception.h"
#include"logger/logger.h"
#include"profiler/profiler.h"
#include"ErrorSystem.h"

namespace gpp {
#undef _GASSERT
#undef _GASSERT_MSG
#define _GASSERT(expr)\
if(!(expr)) {\
logger::fatal("Assertion error in file: {}, Line: {}, Expression: \"{}\"", __FILE__, __LINE__, #expr);\
std::terminate();\
}
        //End macro

#define _GASSERT_MSG(expr, str, ...)\
if(!(expr)) {\
logger::fatal("Assertion error in file: {}, Line: {}, Expression: \"{}\"\nMsg: {}", __FILE__, __LINE__, #expr,         safe_format(str, __VA_ARGS__)); \
        std::terminate(); \
}
//End macro...
}
#endif

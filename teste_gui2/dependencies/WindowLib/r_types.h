

#ifndef R_TYPES_H
#define R_TYPES_H

#include<format>
#include<stdint.h>

typedef int8_t rint8;
typedef uint8_t ruint8;
typedef int16_t rint16;
typedef uint16_t ruint16;
typedef int32_t rint32;
typedef uint32_t ruint32;
typedef int64_t rint64;
typedef uint64_t ruint64;
#if defined(R_BC_64)
typedef ruint64 rwparam;
typedef rint64 rlparam;
#else
typedef ruint32 rwparam;
typedef rint32 rlparam;
#endif
#define r_format(str, ...) std::vformat(str, std::make_format_args(__VA_ARGS__))
#define r_wformat(wstr, ...) std::vformat(wstr, std::make_wformat_args(__VA_ARGS__))
#define r_make_format_args(...) std::make_format_args(__VA_ARGS__)
#define r_make_wformat_args(...) std::make_wformat_args(__VA_ARGS__)
#define r_vformat(str, _args) std::vformat(str, _args)
#endif




#ifndef GPP_TYPES_H
#define GPP_TYPES_H

#include<stdint.h>

namespace gpp
{

typedef uint8_t uint8;
typedef int8_t int8;
typedef uint16_t uint16;
typedef int16_t int16;
typedef uint32_t uint32;
typedef int32_t int32;
typedef uint64_t uint64;
typedef int64_t int64;
#ifndef GPP_DOUBLE_PRECISION
typedef float decimal;
#else
typedef double decimal;
#endif

//classes definitions...

class gpp_EventHandler;
class gpp_PacketHandler;

}
#endif

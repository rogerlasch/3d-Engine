

#ifndef WORDEVENT_H
#define WORDEVENT_H

namespace gpp
{
enum GPP_WORDEVENT_TYPES
{
WE_DEFAULT=0,
WE_STEP,
WE_COLLISION,
WE_ENTERZONE,
WE_LEAVEZONE
};

class WordEvent
{
public:
uint32 type;
void* v1;
void* v2;
WordEvent();
WordEvent(const WordEvent& wv)=delete;
WordEvent& operator=(const WordEvent& v)=delete;
virtual ~WordEvent();
};
}
#endif

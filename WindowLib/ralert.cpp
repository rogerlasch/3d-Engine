

#include"r_window.h"

using namespace std;

namespace rgames
{
ralert::ralert(const string& msg, const string& soundfile):
RdialogBase()
{
this->msg=msg;
this->soundfile=soundfile;
}

ralert::~ralert()
{
}

void ralert::enable(bool en)
{
this->enabled=en;
if(enabled)
{
_speak(msg, false);
}
}

 void ralert::showModal()
{
enable(true);
if(soundfile.size()>0)
{
}
}

 void ralert::endModal()
{
}

 ruint32 ralert::handleEvent(rint32 hstate, rint32 hkey)
{
if(hstate==sk_up) return RD_CONTINUE;
ruint32 x=key_to_event(hstate, hkey);
switch(x)
{
case RA_REPEATE:
{
_speak(msg, true);
break;
}
case RD_FINISH:
{
return RD_FINISH;
}
}
return RD_CONTINUE;
}

 rint32 ralert::key_to_event(rint32 hstate, rint32 hkey)
{
switch(hkey)
{
case SDLK_UP:
case SDLK_DOWN:
case SDLK_LEFT:
case SDLK_RIGHT:
return RA_REPEATE;
case SDLK_ESCAPE:
case SDLK_RETURN:
case ' ':
{
if((rkey_pressed(SDLK_RETURN))||(rkey_pressed(SDLK_ESCAPE))||(rkey_pressed(' ')))
{
return RD_FINISH;
}
}
}
return 0;
}
}

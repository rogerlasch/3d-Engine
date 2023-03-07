


#include<tolk.h>
#include<gpp/gpp.h>
#include"g_window.h"
#include"gspeak.h"

using namespace std;
using namespace gpp;


void gspeak_init()
{
Tolk_Load();
}

void gspeak_unload()
{
Tolk_Unload();
}

void gspeak(const std::string& str, bool interrupt)
{
profiler_snap();
gspeak_w(string_utf8_decode(str), interrupt);
}

void gspeak_w(const std::wstring& s, bool interrupt)
{
profiler_snap();
bool sp=false, st=false;
if(getMainWindow()!=NULL)
{
if(mainWindowHasFocus())
{
sp=true;
}
}
else
{
sp=true;
}
if(sp)
{
Tolk_Speak(s.c_str(), interrupt);
}
}

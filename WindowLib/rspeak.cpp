


#include<tolk.h>
#include"r_types.h"
#include"r_window.h"
#include"rspeak.h"

using namespace std;

namespace rgames
{

void rspeak_init()
{
Tolk_Load();
}

void rspeak_unload()
{
Tolk_Unload();
}

void rspeak(const std::string& str, bool interrupt)
{
rspeak_w(rstring_utf8_decode(str), interrupt);
}

void rspeak_w(const std::wstring& s, bool interrupt)
{
Tolk_Speak(s.c_str(), interrupt);
}
}

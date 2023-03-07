


#include<unordered_map>
#include<gpp/debug_system.h>
#include"gkeyboard.h"

using namespace std;
using namespace gpp;

static unordered_map<int32, int32> keyboard;

void gkeyboard_reset_all(int32 hstate)
{
for(auto it=keyboard.begin(); it!=keyboard.end(); ++it)
{
it->second=hstate;
}
}

void gkey_set_state(int32 hkey, int32 hstate)
{
auto it=keyboard.find(hkey);
if(it==keyboard.end())
{
keyboard.insert(make_pair(hkey, hstate));
}
else
{
it->second=hstate;
}
}

int32 gkey_get_state(int32 hkey)
{
auto it=keyboard.find(hkey);
if(it==keyboard.end())
{
keyboard.insert(make_pair(hkey, sk_up));
return sk_up;
}
return it->second;
}

bool gkey_pressed(int32 key_code)
{
auto it=keyboard.find(key_code);
if(it==keyboard.end())
{
return false;
}
if(it->second==sk_pressed)
{
it->second=sk_down;
return true;
}
return false;
}

bool gkey_down(int32 key_code)
{
auto it=keyboard.find(key_code);
if(it==keyboard.end())
{
return false;
}
if(it->second==sk_pressed)
{
it->second=sk_down;
}
return it->second==sk_down;
}

bool gkey_up(int32 key_code)
{
auto it=keyboard.find(key_code);
return ((it==keyboard.end()) ? true : it->second==sk_up);
}

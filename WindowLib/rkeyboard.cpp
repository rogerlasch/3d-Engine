


#include<unordered_map>
#include"r_types.h"
#include"rkeyboard.h"

using namespace std;

namespace rgames
{
static unordered_map<rint32, rint32> keyboard;

void rkeyboard_reset_all(rint32 hstate)
{
for(auto it=keyboard.begin(); it!=keyboard.end(); ++it)
{
it->second=hstate;
}
}

void rkey_set_state(rint32 hkey, rint32 hstate)
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

rint32 rkey_get_state(rint32 hkey)
{
auto it=keyboard.find(hkey);
if(it==keyboard.end())
{
keyboard.insert(make_pair(hkey, sk_up));
return sk_up;
}
return it->second;
}

bool rkey_pressed(rint32 key_code)
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

bool rkey_down(rint32 key_code)
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

bool rkey_up(rint32 key_code)
{
auto it=keyboard.find(key_code);
return ((it==keyboard.end()) ? true : it->second==sk_up);
}
}

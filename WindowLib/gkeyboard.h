


#ifndef GKEYBOARD_H
#define GKEYBOARD_H

#include<gtypes.h>
using namespace gpp;

enum r_key_state
{
sk_up=0,
sk_pressed,
sk_down
};

void gkeyboard_reset_all(int32 hstate);
void gkey_set_state(int32 hkey, int32 hstate);
int32 gkey_get_state(int32 hkey);
bool gkey_pressed(int32 key_code);
bool gkey_down(int32 key_code);
bool gkey_up(int32 key_code);
#endif

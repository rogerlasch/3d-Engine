


#ifndef RKEYBOARD_H
#define RKEYBOARD_H


namespace rgames
{

enum r_key_state
{
sk_up=0,
sk_pressed,
sk_down
};

void rkeyboard_reset_all(rint32 hstate);
void rkey_set_state(rint32 hkey, rint32 hstate);
rint32 rkey_get_state(rint32 hkey);
bool rkey_pressed(rint32 key_code);
bool rkey_down(rint32 key_code);
bool rkey_up(rint32 key_code);
}
#endif

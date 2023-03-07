


#ifndef GSOUND_H
#define GSOUND_H

#include<gtypes.h>
#include"..\\gpp_physics\\geometry\\vector3d.h"
using namespace gpp;
#include<string>


enum r_sound_flags
{
gsound_default=0,
gsound_autofree=1,
gsound_3d=2,
gsound_loop=4,
gsound_paused=8
};

enum GSOUND_ATRIBUTES
{
gsound_attrib_vol=1,
gsound_attrib_pan
};

bool gsound_device_init(int32 index);
void gsound_device_free(int32 index);
void gsound_device_free_all();
bool gsound_device_started(int32 index);
bool gsound_set_device(int32 index);
int32 gsound_get_device();
int32 gsound_get_default_device_number();
uint32 gsound_play(const std::string& filename, uint32 flags=gsound_default);
uint32 gsound_play(const std::string& filename, uint32 flags, const vector3d& position);
bool gsound_close(uint32 handle);
bool gsound_set_listener_position(vector3d* position, vector3d* velocity=NULL, vector3d* front=NULL, vector3d* top=NULL);
bool gsound_get_listener_position(vector3d& vect);
bool gsound_set_sound_position(uint32 handle, const vector3d& vect);
bool gsound_get_sound_position(uint32 handle, vector3d& vect);
bool gsound_set_attribute(uint32 handle, uint32 atrib, float value);
float gsound_get_attribute(uint32 handle, uint32 atrib);
#endif

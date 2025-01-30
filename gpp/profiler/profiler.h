

#ifndef GPP_PROFILER_H
#define GPP_PROFILER_H

#include"ProfilerManager.h"
namespace gpp{

void profiler_start(uint32 resolution, const std::string& filename);
std::string profiler_dump_string();
void profiler_dump_to_file();
std::shared_ptr<ProfilerObject> profiler_create_object(const std::string& filename);

#define _PROFILER_SNAP() auto _QLZ=profiler_create_object(__FUNCTION__);\
_QLZ->start()

}
#endif


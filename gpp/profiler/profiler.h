
#ifndef GPP_PROFILER_H
#define GPP_PROFILER_H

#include <memory>
#include <string>
#include "ProfilerManager.h"

namespace gpp {

class gpp_profiler {
public:
    static void start(uint32 resolution, const std::string& filename);
    static std::string dump_string();
    static void dump_to_file();

private:
    static std::unique_ptr<ProfilerManager> profilerInstance;
friend class ProfilerObject;
};

#define _PROFILER_SNAP() ProfilerObject _QLZ(__FUNCTION__)
}
#endif

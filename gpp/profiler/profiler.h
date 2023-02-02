

#ifndef GPP_PROFILER_H
#define GPP_PROFILER_H

#include<string>
#ifndef GPP_H
#include"../types.h"
#include"../time.h"
#include"../gpp_value.h"
#endif
#include"ProfilerSnap.h"

namespace gpp
{
void profiler_start();
void profiler_dump(const std::string& filename);
void profiler_log_snap(const std::string& func_name, int64 execution_time_ns);
void profiler_set_int(const std::string& vname, int64 value);
int64 profiler_get_int(const std::string& vname);
void profiler_inc(const std::string& vname, int64 qtd);
void profiler_dec(const std::string& vname, int64 qtd);
void profiler_set_value(const std::string& var_name, const gpp_value& val);
gpp_value profiler_get_value(const std::string& var_name);
void profiler_create_table(const std::string& tname, const std::vector<std::string> header);
void profiler_add_row(const std::string& tname, const std::vector<gpp_value>& values);
#define profiler_snap() ProfilerSnap PROFILER_SNAP_INFO_DATA(__FUNCTION__)
}
#endif



#ifndef PROFILER_H
#define PROFILER_H

#include<string>
#include<vector>
#include"gpp_value.h"

namespace gpp
{

void profiler_start();
void profiler_reset();
void profiler_log(const std::string& func_name, int64 ms);
void profiler_set_int(const std::string& var_name, int64 value);
int64 profiler_get_int(const std::string& var_name);
void profiler_inc(const std::string& var_name, int64 qtd);
void profiler_dec(const std::string& var_name, int64 qtd);
void profiler_create_matrix(const std::string& key_name, const std::vector<std::string>& col_names);
void profiler_add_row(const std::string& key_name, const gpp_value_args& args);
void profiler_dump(const std::string& filename);

class ProfilerSnap
{
private:
std::string func_name;
int64 start;
public:
ProfilerSnap(const std::string& func_name);
~ProfilerSnap();
};

#define profiler_snap() ProfilerSnap SNP(__FUNCTION__)
}
#endif

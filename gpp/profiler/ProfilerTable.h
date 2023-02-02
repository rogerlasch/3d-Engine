


#ifndef GPP_PROFILERTABLE_H
#define GPP_PROFILERTABLE_H


namespace gpp
{

class ProfilerTable
{
public:
std::vector<std::vector<std::string>> htable;
ProfilerTable();
ProfilerTable(const ProfilerTable& pf)=delete;
ProfilerTable& operator=(const ProfilerTable& t)=delete;
void appendHeader(const std::vector<std::string>& h);
void addRow(const std::vector<gpp_value>& row);
std::string toString();
};
typedef std::shared_ptr<ProfilerTable> profiler_table_ptr;
}
#endif

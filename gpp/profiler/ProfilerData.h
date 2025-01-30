
#ifndef GPP_PROFILERDATA_H
#define GPP_PROFILERDATA_H


namespace gpp {

enum PROFILER_RESOLUTION {
    PF_NANO = 0,
    PF_MICRO,
    PF_MS,
    PF_SEC
};

class ProfilerData {
public:
    int64 totalTime;
    int64 minTime;
    int64 maxTime;
    int64 executions;
std::string name;
mutable std::mutex mtx;
    ProfilerData();
    ProfilerData(const ProfilerData& pf);
    ProfilerData& operator=(const ProfilerData& pf);
    ~ProfilerData() = default;

    std::string dump(uint32 resolution) const;
    static double format_time(int64 ns, uint32 resolution);
    void cleanup();
};
} // namespace gpp
#endif // GPP_PROFILERDATA_H

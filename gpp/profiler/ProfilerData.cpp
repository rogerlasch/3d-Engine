
#include"../types.h"
#include<numeric>
#include<limits>
#include <sstream>
#include <iomanip>
#include "ProfilerData.h"

using namespace std;

namespace gpp {

ProfilerData::ProfilerData()
    : name(""),
totalTime(0),
      minTime(std::numeric_limits<int64>::max()),
      maxTime(std::numeric_limits<int64>::min()),
      executions(0) {}

ProfilerData::ProfilerData(const ProfilerData& pf)
    : name(pf.name),
totalTime(pf.totalTime),
      minTime(pf.minTime),
      maxTime(pf.maxTime),
      executions(pf.executions) {}

ProfilerData& ProfilerData::operator=(const ProfilerData& pf) {
    if (this != &pf) {
this->name=pf.name;
        totalTime = pf.totalTime;
        minTime = pf.minTime;
        maxTime = pf.maxTime;
        executions = pf.executions;
    }
    return *this;
}

string ProfilerData::dump(uint32 resolution) const {
ostringstream oss;
    oss << std::fixed << std::setprecision(2);
oss<<"Name: "<<name<<", ";
oss << "Executions: " << executions << ", ";
oss<<"Average Time: "<<format_time((totalTime/executions), resolution)<<", ";
        oss<< "Total Time: " << format_time(totalTime, resolution) << ", "
        << "Min Time: " << format_time(minTime, resolution) << ", "
        << "Max Time: " << format_time(maxTime, resolution);
    return oss.str();
}

double ProfilerData::format_time(int64 ns, uint32 resolution) {
    switch (resolution) {
        case PF_NANO:
            return static_cast<double>(ns);
        case PF_MICRO:
            return ns / 1'000;
        case PF_MS:
            return ns / 1'000'000;
        case PF_SEC:
            return ns / 1'000'000'000;
        default:
            return static_cast<double>(ns); // Default to nanoseconds if resolution is invalid
    }
}

void ProfilerData::cleanup() {
    totalTime = 0;
    minTime = std::numeric_limits<int64>::max();
    maxTime = std::numeric_limits<int64>::min();
    executions = 0;
}
} // namespace gpp

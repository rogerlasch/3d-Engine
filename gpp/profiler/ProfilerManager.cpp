
#include "../types.h"
#include <sstream>
#include <fstream>
#include "ProfilerManager.h"

using namespace std;

namespace gpp {

ProfilerManager::ProfilerManager(uint32 resolution, const string& filename){
this->resolution=resolution;
this->filename=filename;
}

ProfilerManager::~ProfilerManager() {
    for (auto& it : activeProfilers) {
        it->release();
    }
    activeProfilers.clear();

    dumpToFile();
}

shared_ptr<ProfilerObject> ProfilerManager::createProfilerObject(const string& name) {
    auto obj = make_shared<ProfilerObject>(name, this);
    activeProfilers.insert(obj.get());
    return obj;
}

string ProfilerManager::dump() const {
    stringstream ss;
    for (const auto& it : hdata) {
        ss << it.second.dump(resolution) << endl;
    }
    return ss.str();
}

void ProfilerManager::dumpToFile() {
string final=dump();
if(final.size()>0){
    ofstream ofn(filename);
    if (ofn.is_open()) {
        ofn <<final;
        ofn.close();
    }
}
}

void ProfilerManager::registreProfile(const string& name, int64 ts) {
    auto it = hdata.find(name);

    if (it == hdata.end()) {
        ProfilerData data;
        data.executions = 1;
        data.minTime = ts;
        data.maxTime = ts;
        data.totalTime = ts;
data.name=name;
        hdata[name] = data;
    } else {
lock_guard<mutex> lck(it->second.mtx);
        auto& data = it->second;
        data.executions += 1;
        data.minTime = std::min(ts, data.minTime);
        data.maxTime = std::max(ts, data.maxTime);
        data.totalTime += ts;
    }
}

void ProfilerManager::removeObject(ProfilerObject* obj) {
    auto it = activeProfilers.find(obj);
    if (it != activeProfilers.end()) {
        activeProfilers.erase(it);
    }
}
} // namespace gpp

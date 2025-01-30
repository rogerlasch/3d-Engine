

#include"../types.h"
#include"profiler.h"

using namespace std;

namespace gpp {

	static shared_ptr<ProfilerManager> hprofiler;

	void profiler_start(uint32 resolution, const string& filename) {
		if (hprofiler == NULL) {
			hprofiler = make_shared<ProfilerManager>(resolution, filename);
		}
	}

	string profiler_dump_string() {
		if (hprofiler != NULL) {
			return hprofiler->dump();
		}
		return "";
	}

	void profiler_dump_to_file() {
		if (hprofiler != NULL) {
			hprofiler->dumpToFile();
		}
	}

	shared_ptr<ProfilerObject> profiler_create_object(const string& name) {
		if (hprofiler != NULL) {
			return hprofiler->createProfilerObject(name);
		}
		return shared_ptr<ProfilerObject>();
	}
}
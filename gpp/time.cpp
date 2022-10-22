

#include<thread>
#include<chrono>
#include<random>
#include"types.h"
#include"time.h"

using namespace std;

namespace gpp
{
int64 get_timestamp_sec()
{
chrono::system_clock::time_point tp=chrono::system_clock::now();
chrono::system_clock::duration dtn=tp.time_since_epoch();
return chrono::duration_cast<chrono::seconds>(dtn).count();
}

int64 get_timestamp_ms()
{
chrono::system_clock::time_point tp=chrono::system_clock::now();
chrono::system_clock::duration dtn=tp.time_since_epoch();
return chrono::duration_cast<chrono::milliseconds>(dtn).count();
}

int64 get_timestamp_mcs()
{
chrono::system_clock::time_point tp=chrono::system_clock::now();
chrono::system_clock::duration dtn=tp.time_since_epoch();
return chrono::duration_cast<chrono::microseconds>(dtn).count();
}

int64 get_timestamp_ns()
{
chrono::system_clock::time_point tp=chrono::system_clock::now();
chrono::system_clock::duration dtn=tp.time_since_epoch();
return chrono::duration_cast<chrono::nanoseconds>(dtn).count();
}

void wait_ms(uint64 ms)
{
this_thread::sleep_for(chrono::milliseconds(ms));
}

void wait_mcs(uint64 mcs)
{
this_thread::sleep_for(chrono::microseconds(mcs));
}

void wait_ns(uint64 ns)
{
this_thread::sleep_for(chrono::nanoseconds(ns));
}

uint32 random_uint32(uint32 rMin, uint32 rMax)
{
static mt19937 rd(get_timestamp_ms());
static uniform_int_distribution<> distribution(rMin, rMax);
return distribution(rd);
}

uint64 random_uint64(uint64 rMin, uint64 rMax)
{
static mt19937_64 rd(get_timestamp_ms());
static uniform_int_distribution<> distribution(rMin, rMax);
return distribution(rd);
}

float random_float(float rmin, float rmax)
{
random_device randdev;
mt19937 generator(randdev());
uniform_real_distribution<> distrib(rmin, rmax);
return distrib(generator);
}
}



#ifndef GPP_TIME_H
#define GPP_TIME_H

namespace gpp
{

int64 get_timestamp_sec();
int64 get_timestamp_ms();
int64 get_timestamp_mcs();
int64 get_timestamp_ns();
void wait_ms(uint64 ms);
void wait_mcs(uint64 mcs);
void wait_ns(uint64 ns);
uint32 random_uint32(uint32 rMin, uint32 rMax);
uint64 random_uint64(uint64 rMin, uint64 rMax);
float random_float(float rmin, float rmax);
}
#endif

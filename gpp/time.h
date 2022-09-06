

#ifndef GPP_TIME_H
#define GPP_TIME_H

namespace gpp
{

int64 get_time_stamp_sec();
int64 get_time_stamp_ms();
int64 get_time_stamp_mcs();
int64 get_time_stamp_ns();
void wait_ms(uint32 ms);
uint32 random_uint32(uint32 rMin, uint32 rMax);
uint64 random_uint64(uint64 rMin, uint64 rMax);

}
#endif

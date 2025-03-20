

#ifndef GPP_TIME_H
#define GPP_TIME_H

#include<chrono>
#include<functional>
#include<string>


namespace gpp
{

int64 get_timestamp_sec();
int64 get_timestamp_ms();
int64 get_timestamp_mcs();
int64 get_timestamp_ns();
float gpp_nanoseconds_to_milliseconds(int64 ns);
void wait_ms(uint64 ms);
void wait_mcs(uint64 mcs);
void wait_ns(uint64 ns);
uint32 random_uint32(uint32 rMin, uint32 rMax);
uint64 random_uint64(uint64 rMin, uint64 rMax);
float random_float(float rmin, float rmax);

template <typename TimeUnit = std::chrono::milliseconds, typename Func>
std::string gpp_TestFunction(Func&& func) {
    auto start = std::chrono::high_resolution_clock::now();

    func(); // Executa a função passada

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<TimeUnit>(end - start);

std::string result="";
result="Tempo: " + std::to_string(duration.count()) + " "
              + (std::is_same<TimeUnit, std::chrono::seconds>::value ? "segundos" :
                  std::is_same<TimeUnit, std::chrono::milliseconds>::value ? "ms" :
                  std::is_same<TimeUnit, std::chrono::microseconds>::value ?
"ms" :
                  "ns")
              + "\n";

return result;
}

}
#endif

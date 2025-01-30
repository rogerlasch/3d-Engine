

#ifndef GPP_CALENDAR_H
#define GPP_CALENDAR_H

#include <string>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <sstream>

namespace gpp {

class gpp_calendar {
private:
    std::chrono::system_clock::time_point timestamp;
    bool use_utc;
    std::tm current_time;

    void update_internal_time();

public:
    gpp_calendar();
    gpp_calendar(const gpp_calendar& other) = default;
    gpp_calendar& operator=(const gpp_calendar& other) = default;

    bool operator<(const gpp_calendar& other) const;
    bool operator<=(const gpp_calendar& other) const;
    bool operator==(const gpp_calendar& other) const;
    bool operator!=(const gpp_calendar& other) const;
    bool operator>(const gpp_calendar& other) const;
    bool operator>=(const gpp_calendar& other) const;

    void reset();
    int get_year() const;
    int get_month() const;
    int get_day() const;
    int get_hour() const;
    int get_minute() const;
    int get_second() const;
    int get_weekday() const;
    std::string get_weekday_name() const;
    std::string get_month_name() const;
    std::time_t get_unix() const;
    
    bool set(int year, int month, int day, int hour = 0, int minute = 0, int second = 0);
    bool set_unix(std::time_t unix_time);
    void set_utc(bool utc);

    bool is_leap_year() const;
    void add_years(int years);
    void add_months(int months);
    void add_days(int days);
    void add_hours(int hours);
    void add_minutes(int minutes);
    void add_seconds(int seconds);

    int diff_years(const gpp_calendar& other) const;
    int diff_months(const gpp_calendar& other) const;
    int diff_days(const gpp_calendar& other) const;
    int diff_hours(const gpp_calendar& other) const;
    int diff_minutes(const gpp_calendar& other) const;
    int diff_seconds(const gpp_calendar& other) const;

    std::string to_string() const;

    friend std::ostream& operator<<(std::ostream& os, const gpp_calendar& dc);
};

} // namespace gpp

#endif // GPP_CALENDAR_H

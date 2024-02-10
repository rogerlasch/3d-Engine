

/**
*Classe de calendário para lidar facilmente com datas.
*Atualmente ele não lida muito bem com datas anteriores ao ano de 1970
*Escrito por Róger (rogerlasch@gmail.com) em agosto de 2021.
*Este arquivo faz parte da gpp_utils, uma biblioteca de utilidades. Use por sua conta e risco!
**/

#ifndef GPP_CALENDAR_H
#define GPP_CALENDAR_H

#include<string>

namespace gpp
{
class gpp_calendar
{
private:
int64 timestamp;
bool getutc;
tm current_time;
public:
gpp_calendar();
gpp_calendar(const gpp_calendar& dc);
gpp_calendar& operator=(const gpp_calendar& dc);
bool operator<(const gpp_calendar& dc)const;
bool operator<=(const gpp_calendar& dc)const;
bool operator==(const gpp_calendar& dc)const;
bool operator!=(const gpp_calendar& dc)const;
bool operator>(const gpp_calendar& dc)const;
bool operator>=(const gpp_calendar& dc)const;
void reset();
int32 getyear()const;
int32 getmonth()const;
int32 getday()const;
int32 gethour()const;
int32 getminute()const;
int32 getsecond()const;
int32 get_weekday()const;
std::string get_weekday_name()const;
std::string get_month_name()const;
int64 get_unix()const;
bool set(int32 year, int32 month, int32 day, int32 hour, int32 minute, int32 second);
bool set_unix(int64 stime);
void set_utc(bool getutc);
bool leap_year()const;
void add_years(int32 years);
void add_months(int32 months);
void add_days(int32 days);
void add_hours(int32 hours);
void add_minutes(int32 minutes);
void add_seconds(int32 seconds);
int32 diff_years(const gpp_calendar& dc);
int32 diff_months(const gpp_calendar& dc);
int32 diff_days(const gpp_calendar& dc);
int32 diff_hours(const gpp_calendar& dc);
int32 diff_minutes(const gpp_calendar& dc);
int32 diff_seconds(const gpp_calendar& dc);
std::string to_string()const;
friend std::ostream& operator<<(std::ostream& os, const gpp_calendar& dc);
private:
int64 calendar_to_time_t()const;
void adjust_internal();
};

std::ostream& operator<<(std::ostream& os, const gpp_calendar& dc);
}
#endif

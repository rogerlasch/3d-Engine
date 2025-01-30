
#include "gpp_calendar.h"
#include <iostream>
#include <cmath>

namespace gpp {

static const std::vector<std::string> day_names = {"Domingo", "Segunda-feira", "Terça-feira", "Quarta-feira", "Quinta-feira", "Sexta-feira", "Sábado"};
static const std::vector<std::string> month_names = {"Janeiro", "Fevereiro", "Março", "Abril", "Maio", "Junho", "Julho", "Agosto", "Setembro", "Outubro", "Novembro", "Dezembro"};

void gpp_calendar::update_internal_time() {
    std::time_t time = std::chrono::system_clock::to_time_t(timestamp);
    if (use_utc) {
        current_time = *std::gmtime(&time);
    } else {
        current_time = *std::localtime(&time);
    }
}

gpp_calendar::gpp_calendar() : use_utc(false) {
    reset();
}

bool gpp_calendar::operator<(const gpp_calendar& other) const {
    return timestamp < other.timestamp;
}

bool gpp_calendar::operator<=(const gpp_calendar& other) const {
    return timestamp <= other.timestamp;
}

bool gpp_calendar::operator==(const gpp_calendar& other) const {
    return timestamp == other.timestamp;
}

bool gpp_calendar::operator!=(const gpp_calendar& other) const {
    return timestamp != other.timestamp;
}

bool gpp_calendar::operator>(const gpp_calendar& other) const {
    return timestamp > other.timestamp;
}

bool gpp_calendar::operator>=(const gpp_calendar& other) const {
    return timestamp >= other.timestamp;
}

void gpp_calendar::reset() {
    timestamp = std::chrono::system_clock::now();
    update_internal_time();
}

int gpp_calendar::get_year() const {
    return current_time.tm_year + 1900;
}

int gpp_calendar::get_month() const {
    return current_time.tm_mon + 1;
}

int gpp_calendar::get_day() const {
    return current_time.tm_mday;
}

int gpp_calendar::get_hour() const {
    return current_time.tm_hour;
}

int gpp_calendar::get_minute() const {
    return current_time.tm_min;
}

int gpp_calendar::get_second() const {
    return current_time.tm_sec;
}

int gpp_calendar::get_weekday() const {
    return current_time.tm_wday;
}

std::string gpp_calendar::get_weekday_name() const {
    return day_names[current_time.tm_wday];
}

std::string gpp_calendar::get_month_name() const {
    return month_names[current_time.tm_mon];
}

std::time_t gpp_calendar::get_unix() const {
    return std::chrono::system_clock::to_time_t(timestamp);
}

bool gpp_calendar::set(int year, int month, int day, int hour, int minute, int second) {
    std::tm time = {};
    time.tm_year = year - 1900;
    time.tm_mon = month - 1;
    time.tm_mday = day;
    time.tm_hour = hour;
    time.tm_min = minute;
    time.tm_sec = second;

    std::time_t t = std::mktime(&time);
    if (t < 0) return false;

    timestamp = std::chrono::system_clock::from_time_t(t);
    update_internal_time();
    return true;
}

bool gpp_calendar::set_unix(std::time_t unix_time) {
    if (unix_time < 0) return false;
    timestamp = std::chrono::system_clock::from_time_t(unix_time);
    update_internal_time();
    return true;
}

void gpp_calendar::set_utc(bool utc) {
    use_utc = utc;
    update_internal_time();
}

bool gpp_calendar::is_leap_year() const {
    int year = get_year();
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

void gpp_calendar::add_years(int years) {
    current_time.tm_year += years;
    timestamp = std::chrono::system_clock::from_time_t(std::mktime(&current_time));
}

void gpp_calendar::add_months(int months) {
    int years_to_add = months / 12;
    months %= 12;

    if (months + current_time.tm_mon > 11) {
        current_time.tm_year++;
        current_time.tm_mon = (current_time.tm_mon + months) - 12;
    } else {
        current_time.tm_mon += months;
    }

    add_years(years_to_add);
    timestamp = std::chrono::system_clock::from_time_t(std::mktime(&current_time));
}

void gpp_calendar::add_days(int days) {
    timestamp += std::chrono::seconds(days * 86400);
    update_internal_time();
}

void gpp_calendar::add_hours(int hours) {
    timestamp += std::chrono::hours(hours);
    update_internal_time();
}

void gpp_calendar::add_minutes(int minutes) {
    timestamp += std::chrono::minutes(minutes);
    update_internal_time();
}

void gpp_calendar::add_seconds(int seconds) {
    timestamp += std::chrono::seconds(seconds);
    update_internal_time();
}

int gpp_calendar::diff_years(const gpp_calendar& other) const {
    return get_year() - other.get_year();
}

int gpp_calendar::diff_months(const gpp_calendar& other) const {
    return diff_days(other) / 30;
}

int gpp_calendar::diff_days(const gpp_calendar& other) const {
    auto diff = std::chrono::duration_cast<std::chrono::hours>(timestamp - other.timestamp).count();
    return static_cast<int>(diff / 24);
}

int gpp_calendar::diff_hours(const gpp_calendar& other) const {
    return static_cast<int>(std::chrono::duration_cast<std::chrono::hours>(timestamp - other.timestamp).count());
}

int gpp_calendar::diff_minutes(const gpp_calendar& other) const {
    return static_cast<int>(std::chrono::duration_cast<std::chrono::minutes>(timestamp - other.timestamp).count());
}

int gpp_calendar::diff_seconds(const gpp_calendar& other) const {
    return static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(timestamp - other.timestamp).count());
}

std::string gpp_calendar::to_string() const {
    std::ostringstream oss;
    oss << std::put_time(&current_time, "%d de %B de %Y %H:%M:%S");
    return oss.str();
}

std::ostream& operator<<(std::ostream& os, const gpp_calendar& dc) {
    os << dc.to_string();
    return os;
}

} // namespace gpp

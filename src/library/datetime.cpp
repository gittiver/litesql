/* LiteSQL - Date and time classes
 *
 * By Tero Laitinen
 *
 * See LICENSE for copyright information. */
#include "compatibility.hpp"
#include "litesql.hpp"
#include <time.h>
using namespace std;
namespace litesql {

TimeStruct::TimeStruct(time_t t) {
    if (!t)
        t = time(NULL);
#ifndef WIN32
	gmtime_r(&t, &mytm);
#endif
}
int TimeStruct::day() const {
    return mytm.tm_mday;
}
int TimeStruct::dayOfWeek() const {
    return mytm.tm_wday;
}
int TimeStruct::dayOfYear() const {
    return mytm.tm_yday;
}
int TimeStruct::month() const {
    return mytm.tm_mon + 1;
}
int TimeStruct::year() const {
    return mytm.tm_year + 1900;
}
int TimeStruct::hour() const {
    return mytm.tm_hour;
}
int TimeStruct::min() const {
    return mytm.tm_min;
}
int TimeStruct::sec() const {
    return mytm.tm_sec;
}
time_t TimeStruct::timeStamp() const {
    time_t t = mktime(const_cast<struct tm*>(&mytm));
    return t - timezone;
}
TimeStruct& TimeStruct::setDay(int day) {
    mytm.tm_mday = day;
    return *this;
}
TimeStruct& TimeStruct::setMonth(int month) {
    mytm.tm_mon = month - 1;
    return *this;
}
TimeStruct& TimeStruct::setYear(int year) {
    mytm.tm_year = year - 1900;
    return *this;
}
TimeStruct& TimeStruct::setHour(int hour) {
    mytm.tm_hour = hour;
    return *this;
}
TimeStruct& TimeStruct::setMin(int min) {
    mytm.tm_min = min;
    return *this;
}
TimeStruct& TimeStruct::setSec(int sec) {
    mytm.tm_sec = sec;
    return *this;
}
TimeStruct& TimeStruct::setTimeStamp(time_t t) {
#ifndef WIN32
	gmtime_r(&t, &mytm);
#endif
	return *this;
}
void Date::setDate(int year, int month, int day) {
    // setTimeStamp() also calls this
    // TODO: check for dates like 2000-02-31 (not that many days in that month)
    if ((day >= 1) && (day <= 31) && (month >= 1) && (month <= 12)) {
        y = year;
        m = month;
        d = day;
    } else {
        // TODO: Throw exception?
        // Obviously false values
        y = 0;
        d = 0;
        m = 0;
    }
}
Date::Date() {
    setTimeStamp(time(NULL));
}
Date::Date(int day, int month, int year) {
    setDate(year, month, day);
}
Date::Date(time_t t) {
    setTimeStamp(t);
}
Date::Date(string value) {
    // Date in ISO8601-format
    Split data(value, "-");
    // TODO: Better sanity check
    if (data.size() == 3) {
        setDate(atoi(data[0]), atoi(data[1]), atoi(data[2]));
    } else {
        // TODO: Exception ?
    }
}
int Date::day() const {
    return d;
}
int Date::month() const {
    return m;
}
int Date::year() const {
    return y;
}
time_t Date::timeStamp() const {
    TimeStruct t;
    t.setYear(y);
    t.setMonth(m);
    t.setDay(d);
    t.setHour(0);
    t.setMin(0);
    t.setSec(0);
    return t.timeStamp();
}
TimeStruct Date::timeStruct() const {
    return TimeStruct(timeStamp());
}
Date& Date::setDay(int day) {
    if ((day >= 1) && (day <= 31)) {
        d = day;
    } else {
        // TODO: ??
    }
    return *this;
}
Date& Date::setMonth(int month) {
    if ((month >= 1) && (month <= 12)) {
        m = month;
    } else {
        // TODO: ??
    }
    return *this;
}
Date& Date::setYear(int year) {
    y = year;
    return *this;
}
Date& Date::setTimeStamp(time_t t) {
    TimeStruct ts(t);
    setDate(ts.year(), ts.month(), ts.day());
    return *this;
}
string Date::asString(string format) const {
    Split data(format, "%");
    string res = data[0];
    for (size_t i = 1; i < data.size(); i++) {
        string rest = data[i].substr(1, data[i].size());
        switch(data[i][0]) {
            case 'y':
                res += toString(y) + rest;
                break;
            case 'm':
                if (m < 10)
                    res += "0";
                res += toString(m) + rest;
                break;
            case 'd':
                if (d < 10)
                    res += "0";
                res += toString(d) + rest;
                break;
            default:
                res += "error";
                break;
        }
    }
    return res;
}
Time::Time() {
    value = time(NULL) % (24*60*60);
}
Time::Time(int secs) : value(secs) {
}
Time::Time(int hour, int min, int sec) {
    value = hour * 3600 + min * 60 + sec;
}
Time::Time(string value) {
    Split data(value, ":");
    // TODO: Better input validation
    if (data.size() == 3) {
        this->value = atoi(data[0])*3600 + atoi(data[1])*60 + atoi(data[2]);
    } else  {
        // TODO: Exception ?
    }
}
int Time::hour() const {
    return value / 3600;
}
int Time::min() const {
    return (value / 60) % 60;
}
int Time::sec() const {
    return value % 60;
}
int Time::secs() const {
    return value;
}
Time& Time::setHour(int h) {
    value = h * 3600 + min() * 60 + sec();
    return *this;
}
Time& Time::setMin(int m) {
    value = hour() * 3600 + m * 60 + sec();
    return *this;
}
Time& Time::setSec(int s) {
    value = hour() * 3600 + min() * 60 + s;
    return *this;
}
Time& Time::setSecs(int secs) {
    value = secs;
    return *this;
}
string Time::asString(string format) const {
    Split data(format, "%");
    string res = data[0];
    for (size_t i = 1; i < data.size(); i++) {
        string rest = data[i].substr(1, data[i].size());
        switch(data[i][0]) {
            case 'h':
                res += toString(hour()) + rest;
                break;
            case 'M':
                if (min() < 10)
                    res += "0";
                res += toString(min()) + rest;
                break;
            case 's':
                if (sec() < 10)
                    res += "0";
                res += toString(sec()) + rest;
                break;
            default:
                res += "error";
                break;
        }
    }
    return res;
}
DateTime::DateTime() {
    d = Date();
    t = Time();
}
DateTime::DateTime(time_t tt) {
    d = Date(tt);
    t = Time(tt % (24*60*60));
}
DateTime::DateTime(string value) {
    Split data(value, " ");
    if (data.size() == 2) {
        d = Date(data[0]);
        t = Time(data[1]);
    } else {
        //TODO: Exception
    }
}
int DateTime::hour() const {
    return t.hour();
}
int DateTime::min() const {
    return t.min();
}
int DateTime::sec() const {
    return t.sec();
}
int DateTime::day() const {
    return d.day();
}
int Date::dayOfWeek() const {
    return timeStruct().dayOfWeek();
}

int DateTime::dayOfWeek() const {
    return d.dayOfWeek();
}
int DateTime::month() const {
    return d.month();
}
int DateTime::year() const {
    return d.year();
}

time_t DateTime::timeStamp() const {
    return d.timeStamp() + t.secs();
}
TimeStruct DateTime::timeStruct() const {
    return TimeStruct(timeStamp());
}
DateTime& DateTime::setDay(int dd) {
    d.setDay(dd);
    return *this;
}
DateTime& DateTime::setMonth(int m) {
    d.setMonth(m);
    return *this;
}
DateTime& DateTime::setYear(int y) {
    d.setYear(y);
    return *this;
}
DateTime& DateTime::setHour(int h) {
    t.setHour(h);
    return *this;
}
DateTime& DateTime::setMin(int m) {
    t.setMin(m);
    return *this;
}
DateTime& DateTime::setSec(int s) {
    t.setSec(s);
    return *this;
}
string DateTime::asString(string format) const {
    Split data(format, "%");
    string res = data[0];
    for (size_t i = 1; i < data.size(); i++) {
        string rest = data[i].substr(1, data[i].size());
        switch(data[i][0]) {
            case 'y':
                res += toString(d.year()) + rest;
                break;
            case 'm':
                if (d.month() < 10)
                    res += "0";
                res += toString(d.month()) + rest;
                break;
            case 'd':
                if (d.day() < 10)
                    res += "0";
                res += toString(d.day()) + rest;
                break;
            case 'h':
                if (t.hour() < 10)
                    res += "0";
                res += toString(t.hour()) + rest;
                break;
            case 'M':
                if (t.min() < 10)
                    res += "0";
                res += toString(t.min()) + rest;
                break;
            case 's':
                if (t.sec() < 10)
                    res += "0";
                res += toString(t.sec()) + rest;
                break;
            default:
                res += "error";
                break;
        }
    }
    return res;
}
Date Date::operator-(const Date& rhs) {
    Date res;
    res.y = 0;
    res.m = 0;
    res.d = 0;
    res.d += d - rhs.d;
    if (res.d < 0) {
        res.d += 30;
        res.m -= 1;
    }
    res.m += m - rhs.m;
    if (res.m < 0) {
        res.m += 12;
        res.y -= 1;
    }
    res.y += y - rhs.y;
    res.m += 1;
    res.d += 1;
    return res;
}

bool Date::operator>=(const Date& rhs) const {
    if (y>rhs.y) {
        return true;
    } else if (y==rhs.y) {
        if (m>rhs.m) {
            return true;
        } else if (rhs.m==m) {
            if (d>=rhs.d) {
                return true;
            }
        }
    }
    return false;
}

template <>
    Date convert<const string&, Date>(const string& value) {
        return Date(value);
    }
template <>
    Time convert<const string&, Time>(const string& value) {
        return Time(value);
    }
template <>
    DateTime convert<const string&, DateTime>(const string& value) {
        return DateTime(value);
    }
template <>
    Date convert<int, Date>(int value) {
        return Date(value);
    }
template <>
    Time convert<int, Time>(int value) {
        return Time(value);
    }
template <>
    DateTime convert<int, DateTime>(int value) {
        return DateTime(value);
    }
template <>
    Date convert<long, Date>(long value) {
        return Date(value);
    }
template <>
    Time convert<long, Time>(long value) {
        return Time(value);
    }
template <>
    DateTime convert<long, DateTime>(long value) {
        return DateTime(value);
    }

template <>
    std::string convert<const Date&, std::string>(const Date& value) {
        return value.asString();
    }
template <>
    std::string convert<const Time&, std::string>(const Time& value) {
        return value.asString();
    }
template <>
    std::string convert<const DateTime&, std::string>(const DateTime& value) {
        return value.asString();
    }

ostream& operator << (ostream& os, const Date& d) {
    return os << d.asString();
}
ostream& operator << (ostream& os, const Time& d) {
    return os << d.asString();
}
ostream& operator << (ostream& os, const DateTime& d) {
    return os << d.asString();
}
}

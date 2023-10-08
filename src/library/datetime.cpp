/* LiteSQL - Date and time classes
 *
 * The list of contributors at http://litesql.sf.net/
 *
 * See LICENSE for copyright information. */
#include "litesql/datetime.hpp"
#include "compatibility.hpp"
#include <cstdio>
#include <time.h>

using std::ostream;
using std::string;
using std::vector;

namespace litesql {

TimeStruct::TimeStruct(time_t t) {
  if (!t)
    t = time(NULL);
  localtime_r(&t, &mytm);
}
int TimeStruct::day() const { return mytm.tm_mday; }
int TimeStruct::dayOfWeek() const { return mytm.tm_wday; }
int TimeStruct::dayOfYear() const { return mytm.tm_yday; }
int TimeStruct::month() const { return mytm.tm_mon + 1; }
int TimeStruct::year() const { return mytm.tm_year + 1900; }
int TimeStruct::hour() const { return mytm.tm_hour; }
int TimeStruct::min() const { return mytm.tm_min; }
int TimeStruct::sec() const { return mytm.tm_sec; }
time_t TimeStruct::timeStamp() const {
  return mktime(const_cast<struct tm *>(&mytm));
}
TimeStruct &TimeStruct::setDay(int day) {
  mytm.tm_mday = day;
  return *this;
}
TimeStruct &TimeStruct::setMonth(int month) {
  mytm.tm_mon = month - 1;
  return *this;
}
TimeStruct &TimeStruct::setYear(int year) {
  mytm.tm_year = year - 1900;
  return *this;
}
TimeStruct &TimeStruct::setHour(int hour) {
  mytm.tm_hour = hour;
  return *this;
}
TimeStruct &TimeStruct::setMin(int min) {
  mytm.tm_min = min;
  return *this;
}
TimeStruct &TimeStruct::setSec(int sec) {
  mytm.tm_sec = sec;
  return *this;
}
TimeStruct &TimeStruct::setTimeStamp(time_t t) {
  localtime_r(&t, &mytm);
  return *this;
}

Date::Date(time_t t) : value(t) {
  if (value == 0)
    value = time(NULL);
}
Date::Date(int day, int month, int year) {
  value = TimeStruct().setDay(day).setMonth(month).setYear(year).timeStamp();
}
int Date::day() const { return TimeStruct(value).day(); }
int Date::dayOfWeek() const { return TimeStruct(value).dayOfWeek(); }
int Date::month() const { return TimeStruct(value).month(); }
int Date::year() const { return TimeStruct(value).year(); }
time_t Date::timeStamp() const { return value; }
TimeStruct Date::timeStruct() const { return TimeStruct(value); }
Date &Date::setDay(int d) {
  value = TimeStruct(value).setDay(d).timeStamp();
  return *this;
}
Date &Date::setMonth(int m) {
  value = TimeStruct(value).setMonth(m).timeStamp();
  return *this;
}
Date &Date::setYear(int y) {
  value = TimeStruct(value).setYear(y).timeStamp();
  return *this;
}
Date &Date::setTimeStamp(time_t t) {
  value = t;
  return *this;
}
string Date::asString(const string &format) const {
  if (format == "%u") {
    char buf[32];
    snprintf(buf, 32, "%lu", value);
    return buf;
  }
  vector<string> data = split(format, "%");
  TimeStruct ts(value);
  string res = data[0];
  for (size_t i = 1; i < data.size(); i++) {
    string rest = data[i].substr(1, data[i].size());
    switch (data[i][0]) {
    case 'd':
      res += toString(ts.day()) + rest;
      break;
    case 'm':
      res += toString(ts.month()) + rest;
      break;
    case 'y':
      res += toString(ts.year()) + rest;
    }
  }
  return res;
}
DateTime::DateTime(time_t t) { value = (t == 0) ? time(NULL) : t; }


int DateTime::day() const { return timeStruct().day(); }
  
int DateTime::month() const { return timeStruct().month(); }

int DateTime::year() const { return timeStruct().year(); }

int DateTime::hour() const { return timeStruct().hour(); }

int DateTime::min() const { return timeStruct().min(); }

int DateTime::sec() const { return timeStruct().sec(); }

time_t DateTime::timeStamp() const { return value; }

TimeStruct DateTime::timeStruct() const { return TimeStruct(value); }

DateTime &DateTime::setDay(int d) {
  value = TimeStruct(value).setDay(d).timeStamp();
  return *this;
}
DateTime &DateTime::setMonth(int m) {
  value = TimeStruct(value).setMonth(m).timeStamp();
  return *this;
}
DateTime &DateTime::setYear(int y) {
  value = TimeStruct(value).setYear(y).timeStamp();
  return *this;
}
DateTime &DateTime::setHour(int h) {
  value = TimeStruct(value).setHour(h).timeStamp();
  return *this;
}
DateTime &DateTime::setMin(int m) {
  value = TimeStruct(value).setMin(m).timeStamp();
  return *this;
}
DateTime &DateTime::setSec(int s) {
  value = TimeStruct(value).setSec(s).timeStamp();
  return *this;
}
string DateTime::asString(const string &format) const {
  if (format == "%u") {
    char buf[32];
    snprintf(buf, 32, "%lu", value);
    return buf;
  }
  vector<string> data = split(format, "%");
  TimeStruct ts(value);
  string res = data[0];
  for (size_t i = 1; i < data.size(); i++) {
    string rest = data[i].substr(1, data[i].size());
    switch (data[i][0]) {
    case 'd':
      res += toString(ts.day()) + rest;
      break;
    case 'm':
      res += toString(ts.month()) + rest;
      break;
    case 'y':
      res += toString(ts.year()) + rest;
      break;
    case 'h':
      res += toString(ts.hour()) + rest;
      break;
    case 'M':
      if (ts.min() < 10)
        res += "0";
      res += toString(ts.min()) + rest;
      break;
    case 's':
      if (ts.sec() < 10)
        res += "0";
      res += toString(ts.sec()) + rest;
      break;
    }
  }
  return res;
}
template <> Date convert<const string &, Date>(const string &value) {
  return Date(atoi(value));
}
template <> DateTime convert<const string &, DateTime>(const string &value) {
  return DateTime(atoi(value));
}
template <> Date convert<int, Date>(int value) { return Date(value); }
template <> DateTime convert<int, DateTime>(int value) {
  return DateTime(value);
}

template <> DateTime convert<time_t, DateTime>(time_t value) {
  return DateTime(value);
}

template <> std::string convert<const Date &, std::string>(const Date &value) {
  return toString(value.timeStamp());
}
template <>
std::string convert<const DateTime &, std::string>(const DateTime &value) {
  return toString(value.timeStamp());
}

ostream &operator<<(ostream &os, const Date &d) { return os << d.asString(); }

ostream &operator<<(ostream &os, const DateTime &d) {
  return os << d.asString();
}
} // namespace litesql

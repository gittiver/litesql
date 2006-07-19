/* LiteSQL
 *
 * By Tero Laitinen
 *
 * See LICENSE for copyright information. */

#ifndef _litesql_datetime_hpp
#define _litesql_datetime_hpp
/** \file datetime.hpp
    constains Date, Time and DateTime - classes */
#include "field.hpp"
#include <time.h>
namespace litesql {
using namespace std;
/* Holds struct tm. Operates in UTC (or GMT?). */
class TimeStruct {
    struct tm mytm;
public:
    TimeStruct(time_t t=0);
    int day() const;
    int dayOfWeek() const;
    int dayOfYear() const;
    int month() const;
    int year() const;
    int hour() const;
    int min() const;
    int sec() const;
    time_t timeStamp() const;
    TimeStruct& setDay(int day);
    TimeStruct& setMonth(int month);
    TimeStruct& setYear(int year);
    TimeStruct& setHour(int hour);
    TimeStruct& setMin(int min);
    TimeStruct& setSec(int sec);
    TimeStruct& setTimeStamp(time_t t);
};

/** Holds date. Assumes ISO8601 YYYY-MM-DD format. */
class Date {
    /** ISO8601 year. 0001 is 1AD and -0001 is 2BC. */
    int y;
    /** ISO8601 month. 01-12. */
    int m;
    /** ISO8601 day. 01-31. */
    int d;
    /** Performs checking, and sets date, if correct */
    void setDate(int year, int month, int day);
public:
    /** Sets to current date. */
    Date();
    Date(int day, int month, int year);
    /** crops time of day to 00:00:00 */
    Date(time_t t);
    /** ISO8601 */
    Date(string value);
    int day() const;
    int dayOfWeek() const;
    int month() const;
    int year() const;
    time_t timeStamp() const;
    TimeStruct timeStruct() const;

    Date& setDay(int d);
    Date& setMonth(int m);
    Date& setYear(int y);
    Date& setTimeStamp(time_t t);
    string asString(string format="%y-%m-%d") const;
    /** Not in any way precise. Assumes all months to be 30 days. */
    Date operator-(const Date& rhs);
    bool operator>=(const Date& rhs) const;
};
/** Holds time of day. Assumes ISO8601 HH-MM-SS format. */
class Time {
    /** secs after midnight */
    int value;
public:
    /** Sets to current time. */
    Time();
    Time(int secs);
    Time(int hour, int min, int sec);
    /** ISO8601 */
    Time(string value);
    int hour() const;
    int min() const;
    int sec() const;
    int secs() const;
    string asString(string format="%h:%M:%s") const;

    Time& setHour(int d);
    Time& setMin(int m);
    Time& setSec(int y);
    Time& setSecs(int secs);
};
/** Holds date and time of day. Assume ISO8601 YYYY-MM-DD HH-MM-SS format. */
class DateTime {
    Date d;
    Time t;
public:
    /** Sets to current time. */
    DateTime();
    DateTime(time_t);
    /** ISO8601 */
    DateTime(string value);
    int day() const;
    int month() const;
    int year() const;
    int dayOfWeek() const;

    int hour() const;
    int min() const;
    int sec() const;
    time_t timeStamp() const;
    TimeStruct timeStruct() const;
    string asString(string format="%y-%m-%d %h:%M:%s") const;

    DateTime& setDay(int d);
    DateTime& setMonth(int m);
    DateTime& setYear(int y);
    DateTime& setHour(int d);
    DateTime& setMin(int m);
    DateTime& setSec(int y);
    Date& setTimeStamp(time_t t);
};
template <>
Date convert<const string&, Date>(const string& value);
template <>
Time convert<const string&, Time>(const string& value);
template <>
DateTime convert<const string&, DateTime>(const string& value);

template <>
Date convert<int, Date>(int value);
template <>
Time convert<int, Time>(int value);
template <>
DateTime convert<int, DateTime>(int value);

template <>
std::string convert<const Date&, std::string>(const Date& value);
template <>
std::string convert<const Time&, std::string>(const Time& value);
template <>
std::string convert<const DateTime&, std::string>(const DateTime& value);

ostream& operator << (ostream& os, const Date& d);
ostream& operator << (ostream& os, const Time& d);
ostream& operator << (ostream& os, const DateTime& d);
};

#endif

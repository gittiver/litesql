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
namespace litesql {
using namespace std;
/* holds struct tm */
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
        
/** holds date */
class Date {
    time_t value;
public:
    /** crops time of day to 00:00:00 */
    Date(time_t t=0);
    Date(int day, int month, int year);
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
    string asString(string format="%d.%m.%y") const;
};
/** holds time of day */
class Time {
    /** secs after midnight */
    int value;
public:
    Time(int secs=0);
    Time(int hour, int min, int sec);
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
/** holds date and time of day */
class DateTime {
    time_t value;
public:
    DateTime(time_t t=0);
    int day() const;
    int month() const;
    int year() const;
    int hour() const;
    int min() const;
    int sec() const;
    time_t timeStamp() const;
    TimeStruct timeStruct() const;
    string asString(string format="%d.%m.%y %h:%M:%s") const;

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
ostream& operator << (ostream& os, const Date& d);
ostream& operator << (ostream& os, const Time& d);
ostream& operator << (ostream& os, const DateTime& d);
};

#endif

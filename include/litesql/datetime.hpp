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

/** holds date */
class Date {
    time_t value;
public:
    /** crops time of day to 00:00:00 */
    Date(time_t t);
    Date(int day, int month, int year);
    int day();
    int month();
    int year();
    time_t timeStamp();
    Date& setDay(int d);
    Date& setMonth(int m);
    Date& setYear(int y);
    string asString(string format="%d.%m.%y");
};
/** holds time of day */
class Time {
    /** secs after midnight */
    int value;
public:
    Time(int hour, int min, int sec);
    int hour();
    int min();
    int sec();
    int secs();
    Time& setHour(int d);
    Time& setMin(int m);
    Time& setSec(int y);
    Time& setSecs(int secs);
    string asString(string format="%h:%m:%s");
};
/** holds date and time of day */
class DateTime {
    time_t value;
public:
    DateTime(time_t t);
    int hour();
    int min();
    int sec();
    time_t timeStamp();
    DateTime& setDay(int d);
    DateTime& setMonth(int m);
    DateTime& setYear(int y);
    DateTime& setHour(int d);
    DateTime& setMin(int m);
    DateTime& setSec(int y);
    
    string asString(string format="%d.%m.%y %h:%m:%s");
};
};

#endif

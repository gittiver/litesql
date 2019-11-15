
#include <iostream>
#include "litesql/logger.hpp"

using namespace litesql;

using std::string;

using std::cout;
using std::cerr;
using std::endl;

static bool logger_verbose = false;

void Logger::verbose(bool verbose)
{
  logger_verbose = verbose;
}

void Logger::report(const string& msg,const string& msg2)
{
  if (logger_verbose)
    cout  << msg.c_str()  << msg2.c_str() << endl;
}

void Logger::report(const string& msg)
{
    if (logger_verbose)
    {
        cout  << msg.c_str() << endl;
    }
}

void Logger::error(const std::string& msg)
{
    cerr  << msg.c_str() << endl;
}

void Logger::error(const std::string& msg,const std::string& msg2)
{
    cerr  << msg.c_str() << msg2.c_str() << endl;
}

void Logger::error(const Except& ex)
{
    cerr  << ex << endl;
}



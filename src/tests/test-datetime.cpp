/* LiteSQL - test-datetime
 * 
 * The list of contributors at http://litesql.sf.net/ 
 * 
 * See LICENSE for copyright information. */

#include <cassert>
#include "litesql/datetime.hpp"
/*
  Datetime unit tester

  TC1: test for equality on load/ save (see ticket #13)
*/
using namespace litesql;
using std::string;

int main(int /*argc*/, char * /*argv*/[]) {
    // TC1 for DateTime
   DateTime dt;
   std::string dtstring = dt.asString();

   DateTime dt2 = convert<const string&, DateTime>(dtstring);
   
   assert(dt.timeStamp() == dt2.timeStamp()); 

   // TC1 for Date
   Date d;
   std::string dstring = d.asString();

   Date d2 = convert<const string&, Date>(dstring);
   
   assert(d.timeStamp() == d2.timeStamp()); 

   return 0;
}

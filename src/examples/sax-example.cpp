// include LiteSQL's header file and generated header file
#include "objectmodel.hpp"
#include <iostream>

using namespace std;

static bool verbose = true;
void report(const string& msg) {
    if (verbose)
        cout << msg;
}

int main(int argc, char **argv) {
   ObjectModel model;
   model.loadFromFile("exampledatabase.xml");
   return 0;
}

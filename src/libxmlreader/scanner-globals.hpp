#ifndef _xmlreader_scanner_globals_h
#define _xmlreader_scanner_globals_h
#include <list>
#include <string>
#include "flexml-header.hpp"
#include "litesql/xmltypes.hpp"
namespace xml {
    extern std::list<Position> posStack;
    extern std::string currentFile;
}



#endif



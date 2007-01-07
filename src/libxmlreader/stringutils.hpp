#ifndef _xmlreader_stringutils_hpp
#define _xmlreader_stringutils_hpp

#include <string>
namespace xml {

    std::string capitalize(const std::string& s);
    std::string decapitalize(const std::string& s);
    std::string safe(const char * s);
    std::string makeDbName(const std::string& s);
}
#endif

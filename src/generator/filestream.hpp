#ifndef _litesql_filestream_hpp
#define _litesql_filestream_hpp

#include <map>
#include <string>
#include "litesql/string.hpp"

namespace gen {
    class FileStream {
        typedef std::map< std::string, std::string> Data;
        Data data;        
        std::string currentFile;
    public:

        FileStream& file(const std::string& name);
        FileStream& operator << (const std::string& s);

        template <class T>
        FileStream& operator << (const T& s) {
            return *this << litesql::toString(s);
        }

        void sync() const;
    };
}

#endif

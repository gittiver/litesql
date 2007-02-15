#include "stringutils.hpp"
#include "md5.hpp"
namespace xml {

    using namespace std;

    string capitalize(const string& s) {

        if (s.size() == 0)
            return s;
        char buf[2] = {toupper(s[0]), 0};
        return string(buf) + s.substr(1, s.size());
    }

    string decapitalize(const string& s) {

        if (s.size() == 0)
            return s;
        char buf[2] = {tolower(s[0]), 0};
        return string(buf) + s.substr(1, s.size());
    }

    string safe(const char* s) {

        if (s)
            return s;
        return "";
    }

    string makeDbName(const string& s) {
        if (s.size() > 31)
            return "_" + md5HexDigest(s);
    }

}


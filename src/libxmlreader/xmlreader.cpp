#include "xmlreader.hpp"
#include "xmlreader-actions.hpp"
#include "scanner.hpp"

namespace xml {
    extern Database* xmlReaderDb;

    using namespace std;

    Database* parse(const string& fName) {
        yyin = fopen(fName.c_str(), "r");
        if (!yyin)
            throw litesql::Except("Could not open file: " + fName);
        
        
        yylex();
        fclose(yyin);

        return xmlReaderDb;
    }
}


#include <cstdio>
#include "litesql.hpp"
#include "litesql-gen-cpp.hpp"
using namespace std;

char* help = 
"Usage: litesql-gen [options] <my-database.xml>\n\n"
"Options:\n"
" -l, --target=TARGET         generate code for TARGET (default: c++)\n"
" -v, --verbose               verbosely report code generation\n"
" --help                      print help\n"
"\n"
"Supported targets:\n"
"  'c++'    C++ target (.cpp,.hpp)\n"
//"  'c'      C target (.c,.h)\n"
//"  'hs'     Haskell target (.hs)\n"
//"  'sql'    SQL schema of database (.sql)\n"
//"  'gviz'   Graphviz file (.dot)\n"
"\n\n"
;

static string language = "c++";
static bool verbose = false;
void report(const string& msg) {
    if (verbose)
        cout << msg;
}
void generateCode(xml::Database& db,
                  vector<xml::Object>& objects,
                  vector<xml::Relation>& relations) {
    xml::init(db, objects, relations);
    if (language == "c++") 
        writeCPPClasses(db, objects, relations);
    else
        throw litesql::Except("unsupported language: " + language);
}

int litesql_gen_main(int argc, char **argv, FILE ** yyin) { 
    bool printHelp = false;
    for (size_t i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "-v" || arg == "--verbose") {
            verbose = true;
            continue;
        } else if (arg == "-l" || arg == "--language") {
            if (i+1 >= argc) {
                fprintf(stderr, "Error: missing language\n");
                return -1;
            }    
            language = argv[i+1];
            i++;
            continue;
        } else if (litesql::startsWith(arg, "--language=")) {
            litesql::Split lang(arg, "=");
            language = lang[1];
            continue;
        } else if (arg == "--help") {
            printHelp = true;
            break;
        } else if (i < argc - 1) {
            fprintf(stderr, "Error: invalid argument '%s'", arg.c_str());
            return -1;
        }
    }
    if (argc == 1 || printHelp) {
        fprintf(stderr, help);
        return -1;
    }
    *yyin = fopen(argv[argc-1], "r");
    if (!*yyin) {
        string msg = "could not open file '" + string(argv[argc-1]) + "'";
        perror(msg.c_str());
        return -1;
    }
    try {
        return yylex();
    } catch (litesql::Except e) {
        cerr << "Error: " << e << endl;
        return -1;
    }
}

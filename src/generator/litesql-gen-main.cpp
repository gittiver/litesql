#include <cstdio>
#include "litesql.hpp"
#include "litesql-gen-cpp.hpp"
#include "litesql-gen-graphviz.hpp"
using namespace std;

char* help = 
"Usage: litesql-gen [options] <my-database.xml>\n\n"
"Options:\n"
" -t, --target=TARGET         generate code for TARGET (default: c++)\n"
" -v, --verbose               verbosely report code generation\n"
" --help                      print help\n"
"\n"
"Supported targets:\n"
"  'c++'        C++ target (.cpp,.hpp)\n"
//"  'c'          C target (.c,.h)\n"
//"  'haskell'    Haskell target (.hs)\n"
//"  'sql'        SQL schema of database (.sql)\n"
//"  'php'        PHP target (.php)\n"
//"  'python'     Python target (.py)\n"
"  'graphviz'   Graphviz file (.dot)\n"
"\n\n"
;

static string target = "c++";
static bool verbose = false;
void report(const string& msg) {
    if (verbose)
        cout << msg;
}
void generateCode(xml::Database& db,
                  vector<xml::Object*>& objects,
                  vector<xml::Relation*>& relations) {
    xml::init(db, objects, relations);
    if (target == "c++") 
        writeCPPClasses(db, objects, relations);
    else if (target == "graphviz") 
        writeGraphviz(db, objects, relations);
    else
        throw litesql::Except("unsupported target: " + target);
}

int litesql_gen_main(int argc, char **argv, FILE ** yyin) { 
    bool printHelp = false;
    for (size_t i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "-v" || arg == "--verbose") {
            verbose = true;
            continue;
        } else if (arg == "-t" || arg == "--target") {
            if (i+1 >= argc) {
                fprintf(stderr, "Error: missing target\n");
                return -1;
            }    
            target = argv[i+1];
            i++;
            continue;
        } else if (litesql::startsWith(arg, "--target=")) {
            litesql::Split lang(arg, "=");
            target = lang[1];
            continue;
        } else if (arg == "--help") {
            printHelp = true;
            break;
        } else if (i < argc - 1) {
            fprintf(stderr, "Error: invalid argument '%s'\n", arg.c_str());
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

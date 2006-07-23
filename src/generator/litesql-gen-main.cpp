#include <cstdio>
#include "litesql.hpp"
#include "litesql-gen-cpp.hpp"
#include "litesql-gen-graphviz.hpp"
#include "litesql-gen-python.hpp"
#include "scanner.hpp"
using namespace std;

char* help = 
"Usage: litesql-gen [options] <database.xml>\n\n"
"Options:\n"
" -t TARGET, --target=TARGET  generate code for TARGET (default: c++)\n"
" -v, --verbose               verbosely report code generation\n"
" --prepend-file=FILE         prepend file FILE to every generated file\n"
" -d DIR, --dir=DIR           write generated files to directory DIR\n"
" --cppext=EXT                define extension for C++ source files (cpp)\n"
" --hppext=EXT                define extension for C++ header files (hpp)\n"
" -h, --help                  print help\n"
"\n"
"Supported targets:\n"
"  'c++'        C++ target (.cpp,.hpp)\n"
//"  'c'          C target (.c,.h)\n"
//"  'haskell'    Haskell target (.hs)\n"
//"  'sql'        SQL schema of database (.sql)\n"
//"  'php'        PHP target (.php)\n"
"  'python'     Python target (.py)\n"
"  'graphviz'   Graphviz file (.dot)\n"
"\n\n"
;

static Args* args = NULL;
static bool verbose = false;
void report(const string& msg) {
    if (verbose)
        cout << msg;
}
void generateCode(xml::Database& db) {
    if (!args)
        throw litesql::Except("no arguments");
    string target = (*args)["target"];
    xml::init(db, *args);
    if (target == "c++") 
        writeCPPClasses(db, *args);
    else if (target == "python")
        writePython(db, *args);
    else if (target == "graphviz") 
        writeGraphviz(db, *args);
    else
        throw litesql::Except("unsupported target: " + target);
}
int parseFile(const string& name, FILE** yyin) {
    *yyin = fopen(name.c_str(), "r");
    currentFile = name;
    if (!*yyin) {
        string msg = "could not open file '" + name + "'";
        throw litesql::Except(msg); // + ":" + strerror(errno));
    }
    int ret = yylex();
    return ret;
}
int litesql_gen_main(int argc, char **argv, FILE ** yyin) { 
    bool printHelp = false;
    args = new Args(argc, argv);
    try {
         (*args)
            .option("verbose", "-v", "--verbose")
            .option("target", "-t", "--target", true, false, "c++")
            .option("help", "-h", "--help")
            .option("prepend-file", "", "--prepend-file", true)
            .option("dir", "-d", "--dir", true)
            .option("cppext", "", "--cpp-ext", true, false, "cpp")
            .option("hppext", "", "--hpp-ext", true, false, "hpp")
            .parse();
    } catch (litesql::Except e) {
        cerr << e << endl;
        return -1;
    }
    if (args->has("help")) {
        fprintf(stderr, help);
        return -1;
    }
    if (args->has("verbose"))
        verbose = true;
    litesql::Split params = args->getParams();
    if (params.size() != 1) {
        cerr << "Usage: litesql-gen [options] <database.xml>" << endl << endl;
        return -1;
    }
    try {
        parseFile(argv[argc-1], yyin);
    } catch (litesql::Except e) {
        cerr << "Error: " << e << endl;
        return -1;
    }
}

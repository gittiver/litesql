#include "litesql.hpp"
#include "generator.hpp"
#include "litesql-gen-cpp.hpp"
#include "litesql-gen-graphviz.hpp"
#include "logger.hpp"
#include "objectmodel.hpp"

using namespace std;

const char* help = 
"Usage: litesql-gen [options] <my-database.xml>\n\n"
"Options:\n"
" -t, --target=TARGET         generate code for TARGET (default: c++)\n"
" -v, --verbose               verbosely report code generation\n"
" --help                      print help\n"

" -t, --target=TARGET         generate code for TARGET (default: c++)\n"

"\n"
"Supported targets:\n"
"  'c++'        C++ target (.cpp,.hpp)\n"
"  'ruby'          ruby target (.rb)\n"
"  'objc'          Objective C (.m,.h)\n"
//"  'c'          C target (.c,.h)\n"
//"  'haskell'    Haskell target (.hs)\n"
//"  'sql'        SQL schema of database (.sql)\n"
//"  'php'        PHP target (.php)\n"
//"  'python'     Python target (.py)\n"
"  'graphviz'   Graphviz file (.dot)\n"
"\n\n"
;

vector<string> targets;

int generateCode(ObjectModel& model)
{
  try {
    CompositeGenerator generator;
    for (vector<string>::const_iterator target= targets.begin(); target!=targets.end();target++)
    {

      if (*target == "c++") 
      {
        generator.add(new CppGenerator());
      }    
      else if (*target == "graphviz") 
      {
        generator.add(new GraphvizGenerator());
      }
      else 
      {
        throw litesql::Except("unsupported target: " + *target);
      }
    }

    generator.setOutputDirectory("c:");
    generator.generateCode(&model);
    return 0;
  }
  catch (Except e) {
    cerr << e << endl;
    return -1;
  }
}

int main(int argc, char **argv) { 
    bool printHelp = false;
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "-v" || arg == "--verbose") {
          Logger::verbose(true);
            continue;
        } else if (arg == "-t" || arg == "--target") {
            if (i+1 >= argc) {
              Logger::error("Error: missing target");
                return -1;
            }    
            targets.push_back(argv[i+1]);
            i++;
            continue;
        } else if (litesql::startsWith(arg, "--target=")) {
            litesql::Split lang(arg, "=");
            targets.push_back(lang[1]);
            continue;
        } else if (arg == "--help") {
            printHelp = true;
            break;
        } else if (i < argc - 1) {
          Logger::error("Error: invalid argument "+ arg);
            return -1;
        }
    }
    
    if (argc == 1 || printHelp) {
      Logger::error(help);
        return -1;
    }

    ObjectModel model;
    if (!model.loadFromFile(argv[argc-1]))
    {
        string msg = "could not load file '" + string(argv[argc-1]) + "'";
        Logger::error(msg);
        return -1;
    }
    else
    {
      return generateCode(model);    
    }
}

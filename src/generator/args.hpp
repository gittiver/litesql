#ifndef _litesql_gen_args_hpp
#define _litesql_gen_args_hpp
#include <string>
#include <vector>
#include <map>
#include "litesql.hpp"
class Args {
    class Option {
    public:
        std::string key, shortName, longName, default_;
        bool required, hasParam;
        Option() {}
        Option(std::string k, std::string s, 
               std::string l, bool hasP, bool req,
               std::string def)
            : key(k), shortName(s),
              longName(l), default_(def), 
              required(req), hasParam(hasP) {}
    };
    litesql::Split argv;
    std::map<std::string, Option> options;
    std::map<std::string, std::string> shortNames;
    std::map<std::string, std::string> longNames;
    // key -> params mapping
    std::map<std::string, std::string> params;
    std::vector<std::string> required;
    litesql::Split unbound;
public:
    
    
    Args(int argc, char **argv);
    Args& option(std::string key,
                 std::string shortName,
                 std::string longName,
                 bool hasParam=false, 
                 bool required=false,
                 std::string def="");
    void parse();
    bool has(std::string key);
    std::string operator [] (std::string key);
    // unbound params
    litesql::Split getParams();
};
#endif

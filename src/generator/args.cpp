#include "args.hpp"
#include <list>
using namespace std;
using namespace litesql;
Args::Args(int argc, char **argv) {
    for (int i = 1; i < argc; i++) 
        this->argv.push_back(argv[i]);
}
Args& Args::option(string key, string sn, string ln, 
                   bool p, bool req, string d) {
    options[key] = Option(key, sn, ln, p, req, d);
    if (req)
        required.push_back(key);
    if (!sn.empty())
        shortNames[sn] = key;
    longNames[ln] = key;
    return *this;
}
void Args::parse() {
    // 1. luetaan token
    // 2. jos tunnistetaan option-vivuksi,
    //    luetaan pnum - seuraavaa parametria
    //    optionille, jos ei, siirret‰‰n unbound
    //    parametreiksi
    //
    list<string> args;
    for (size_t i = 0; i < argv.size(); i++)
        args.push_back(argv[i]);

    while (args.size() > 0) {
        string token = args.front();
        string lnToken = Split(token, "=")[0];
        args.pop_front();
        if (shortNames.find(token) != shortNames.end()) {
            Option& opt = options[shortNames[token]];
            if (params.find(opt.key) != params.end())
                throw Except("duplicate option : " + token);
            if (args.size() == 0 && opt.hasParam)
                throw Except("missing parameter for option : " + token);
            if (opt.hasParam) {
                params[opt.key] = args.front(); 
                args.pop_front();
            }
            else
                params[opt.key] = "true";
            continue;
        } else if (longNames.find(lnToken) != longNames.end()) {
            Option& opt = options[longNames[lnToken]];
            if (params.find(opt.key) != params.end())
                throw Except("duplicate option : " + lnToken);
            if (args.size() == 0 && opt.hasParam && token.find("=") == string::npos)
                throw Except("missing parameter for option : " + lnToken);
            if (opt.hasParam) {
                if (token.find("=") != string::npos)
                    params[opt.key] = Split(token, "=")[1];
                else {
                    params[opt.key] = args.front();
                    args.pop_front();
                }
            } 
            else
                params[opt.key] = "true";
            continue;
        }
        unbound.push_back(token);
    }
    for (size_t i = 0; i < required.size(); i++)
        if (!has(required[i]))
            throw Except("missing required parameter : " + required[i]);  
}
bool Args::has(string key) {
    if (options.find(key) == options.end())
        return false;
    if (params.find(key) != params.end())
        return true;
    return !options[key].default_.empty();
}
string Args::operator[](string key) {
    if (has(key)) {
        if (params.find(key) != params.end())
            return params[key];
        else
            return options[key].default_;
    }
    else
        return "";
}
Split Args::getParams() {
    return unbound;
}


#ifndef _litesql_generator_hpp
#define _litesql_generator_hpp
#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include "litesql/string.hpp"
#include "filestream.hpp"

namespace gen {
using namespace litesql;
using namespace std;

class Variable {
public:
    string name, type, value;
    bool isStatic, isProtected;
    
    Variable(string n, string t, string v="") 
        : name(n), type(t), value(v), isStatic(false), isProtected(false) {}
    Variable& static_() {
        isStatic = true;
        return *this;
    }

    Variable& protected_() {
        isProtected = true;
        return *this;
    }
    
    string classDeclaration() {
        char buf[512];
        snprintf(buf, 512, "%s%s %s;", isStatic ? "static " : "",
                 type.c_str(), name.c_str());
        return buf;
    }
    string quotedValue() {
        if (type == "string")
            return "\"" + value + "\"";
        return value;
    }
    string paramDefinition(bool defaults=true) {
        char buf[512];
        
        string val;
        if (defaults && value.size() > 0)
            val = "=" + quotedValue();
        
        snprintf(buf, 512, "%s %s%s", 
                 type.c_str(), name.c_str(), val.c_str());
        return buf;
    }
    string staticDefinition(string context) {
        char buf[512];
        string val = quotedValue();
        string params;
        if (value.size() > 0) {
            params = "(" + val + ")";
        }
        
        snprintf(buf, 512, "%s %s%s%s;", 
                 type.c_str(), context.c_str(), name.c_str(), params.c_str());
        return buf;
    }
};
#include <iostream>
class Method {
public:
    string name, returnType, templateParams, constructorParams;
    bool isStatic, isVirtual, isProtected, isTemplate, isTemplateSpec, isDefinition, isConst, isAbstract, isExplicit, isInline;
    vector<Variable> params;
    vector<string> bodyLines;
   
    Method(const string& n, const string& rt="") 
        : name(n), returnType(rt), templateParams(""), constructorParams(""),
          isStatic(false), isVirtual(false), isProtected(false), isTemplate(false),
          isTemplateSpec(false), isDefinition(false), isConst(false), isAbstract(false), isExplicit(false), isInline(false) {}
    Method& param(const Variable& v) {
        params.push_back(v);
        return *this;
    }
    Method& constructor(string params) {
        constructorParams = params;
        return *this;
    }
    Method& body(string line) {
        bodyLines.push_back(line);
        return *this;
    }
    Method& static_() {
        isStatic = true;
        return *this;
    }
    Method& virtual_() {
        isVirtual = true;
        return *this;
    }
    Method& protected_() {
        isProtected = true;
        return *this;
    }
    Method& defineOnly() {
        isDefinition = true;
        return *this;
    }
    Method& template_(string params) {
        isTemplate = true;
        templateParams = params;
        return *this;
    }
    Method& templateSpec(string params) {
        isTemplate = isTemplateSpec = true;
        templateParams = params;
        return *this;
    }
    Method& const_() {
        isConst = true;
        return *this;
    }
    Method& abstract_() {
        isAbstract = true;
        return *this;
    }
    Method& explicit_() {
        isExplicit = true;
        return *this;
    }
    Method& inline_() {
        isInline = true;
        return *this;
    }

    void write(FileStream& hpp, FileStream& cpp, string context, int indent=0) {
        string ind = string(" ") * indent;
        string expl;
        string tpl;
        string sta;
        string ret;
        string cons;
        string inl;
        if (isExplicit)
           expl = "explicit "; 
        if (isTemplate) 
            tpl = "template <" + templateParams + "> ";
        if (isInline)
            inl = "inline ";
        if (isStatic)
            sta = "static ";
        if (isVirtual)
            sta = "virtual ";
        if  (returnType.size()) 
            ret = returnType + " ";
        if (constructorParams.size())
            cons = "\n" + ind + " : " + constructorParams;
        string cnst;
        if (isConst)
            cnst = " const";
        if (isAbstract)
            cnst += "=0";
        string paramString;
        for (size_t i = 0; i < params.size(); i++) {
            if (i > 0)
                paramString += ", ";
            paramString += params[i].paramDefinition(isTemplateSpec==false);
        }
        string result;
        
        if (!isTemplateSpec) 
            hpp << ind << expl << tpl << inl << sta
                << ret << name << brackets(paramString) << cnst;
        
        if (isDefinition) 
            hpp << ";\n";
        else {
            if ((isTemplate && !isTemplateSpec) || isInline) {
                
                hpp << cons << " {\n";
                for (size_t i = 0; i < bodyLines.size(); i++) 
                    hpp << ind << "    " << bodyLines[i] << "\n";
                hpp << ind << "}\n";
            } else {
                hpp << ";\n";
                paramString = "";
                for (size_t i = 0; i < params.size(); i++) {
                    if (i > 0)
                        paramString += ", ";
                    paramString += params[i].paramDefinition(false);
                }
                ind = string(" ") * 4;
                cpp << tpl << ret << context << name << brackets(paramString)
                    << cnst << cons << " {\n";
                    
                for (size_t i = 0; i < bodyLines.size(); i++) 
                    cpp << ind << bodyLines[i] << "\n";
                cpp << "}\n";                    
            }
        }
    }
};
class Class {
    string name;
    string inherits;
    vector<Method> methods;
    vector<Variable> variables;
    vector<Class> classes;
    vector<string> typedefs;
public:
    Class(string n, string i="") : name(n), inherits(i) {}
    Class& typedef_(const string what, const string is) {
        typedefs.push_back("typedef " + is + " " + what);
        return *this;
    }
    Class& method(const Method& m) {
        methods.push_back(m);
        return *this;
    }
    Class& variable(const Variable& v) {
        variables.push_back(v);
        return *this;
    }
    Class &class_(const Class& c) {
        classes.push_back(c);
        return *this;
    }
    void write(FileStream& hpp, FileStream& cpp, 
               int indent=0, string context="") {
        string inh;
        string ind = string(" ") * indent;
        context += name + "::";

        if (inherits != "") 
            inh = " : public " + inherits;
        string mode = "public";
        hpp << ind << "class " << name << inh << " {\n"
            << ind << "public:\n";

        for (size_t i = 0; i < classes.size(); i++) 
            hpp << ind << "    class " << classes[i].name << ";\n";

        for (size_t i = 0; i < typedefs.size(); i++)
            hpp << ind << "    " << typedefs[i] << ";\n";

        for (size_t i = 0; i < classes.size(); i++) 
            classes[i].write(hpp, cpp, indent + 4, context);
        
        for (size_t i = 0; i < variables.size(); i++)  {
            string decl = variables[i].classDeclaration();
            if (variables[i].isProtected && mode=="public") {
                mode = "protected";
                hpp << ind << "protected:\n";
            }
            if (!variables[i].isProtected && mode=="protected") {
                mode = "public";
                hpp << ind << "public:\n";
            }
                
            hpp << "    " << ind << decl;
            if (variables[i].isStatic) {
                string def = variables[i].staticDefinition(context);
                cpp << def << "\n";
            }
        }
        for (size_t i = 0; i < methods.size(); i++) {
            if (methods[i].isProtected && mode=="public") {
                mode = "protected";
                hpp << ind << "protected:\n";
            }
            if (!methods[i].isProtected && mode=="protected") {
                mode = "public";
                hpp << ind << "public:\n";
            }
            methods[i].write(hpp, cpp, context, indent + 4);            
        }
        hpp << ind << "};\n";
    }
};

}
#endif

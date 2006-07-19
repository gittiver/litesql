#ifndef _litesql_generator_hpp
#define _litesql_generator_hpp
#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include "litesql/string.hpp"
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
    bool isStatic, isVirtual, isProtected, isTemplate, isTemplateSpec, isDefinition, isConst, isAbstract;
    vector<Variable> params;
    vector<string> bodyLines;
   
    Method(const string& n, const string& rt="") 
        : name(n), returnType(rt), templateParams(""), constructorParams(""),
          isStatic(false), isVirtual(false), isProtected(false), isTemplate(false),
          isTemplateSpec(false), isDefinition(false), isConst(false), isAbstract(false) {}
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

    void write(FILE * hpp, FILE * cpp, string context, int indent=0) {
        string ind = string(" ") * indent;
        string tpl;
        string sta;
        string ret;
        string cons;
        if (isTemplate) 
            tpl = "template <" + templateParams + "> ";
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
            fprintf(hpp, "%s%s%s%s%s(%s)%s", ind.c_str(), tpl.c_str(), sta.c_str(),
                    ret.c_str(), name.c_str(), paramString.c_str(), cnst.c_str());
        if (isDefinition) 
            fprintf(hpp, ";\n");
        else {
            if (isTemplate && !isTemplateSpec) {
                fprintf(hpp, " {\n");
                for (size_t i = 0; i < bodyLines.size(); i++) 
                    fprintf(hpp, "%s%s\n", ind.c_str(), bodyLines[i].c_str());
                fprintf(hpp, "%s}\n", ind.c_str());
            } else {
                fprintf(hpp, ";\n");
                paramString = "";
                for (size_t i = 0; i < params.size(); i++) {
                    if (i > 0)
                        paramString += ", ";
                    paramString += params[i].paramDefinition(false);
                }
                ind = string(" ") * 4;
                fprintf(cpp, "%s%s%s%s(%s)%s%s {\n", tpl.c_str(), ret.c_str(),
                        context.c_str(), name.c_str(), paramString.c_str(),
                        cnst.c_str(), cons.c_str());
                for (size_t i = 0; i < bodyLines.size(); i++) 
                    fprintf(cpp, "%s%s\n", ind.c_str(), bodyLines[i].c_str());            
                fprintf(cpp, "}\n");
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
public:
    Class(string n, string i="") : name(n), inherits(i) {}
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
    void write(FILE * hpp, FILE * cpp, int indent=0, string context="") {
        string inh;
        string ind = string(" ") * indent;
        context += name + "::";

        if (inherits != "") 
            inh = " : public " + inherits;
        string mode = "public";
        fprintf(hpp, "%sclass %s%s {\n", ind.c_str(), name.c_str(), inh.c_str());
        fprintf(hpp, "%spublic:\n", ind.c_str());
        for (size_t i = 0; i < classes.size(); i++) {

            classes[i].write(hpp, cpp, indent + 4, context);
        }
        for (size_t i = 0; i < variables.size(); i++)  {
            string decl = variables[i].classDeclaration();
            if (variables[i].isProtected && mode=="public") {
                mode = "protected";
                fprintf(hpp, "%sprotected:\n", ind.c_str());
            }
            if (!variables[i].isProtected && mode=="protected") {
                mode = "public";
                fprintf(hpp, "%spublic:\n", ind.c_str());
            }
                
            fprintf(hpp, "    %s%s\n", ind.c_str(), decl.c_str());
            if (variables[i].isStatic) {
                string def = variables[i].staticDefinition(context);
                fprintf(cpp, "%s\n", def.c_str());
            }
        }
        for (size_t i = 0; i < methods.size(); i++) {
            if (methods[i].isProtected && mode=="public") {
                mode = "protected";
                fprintf(hpp, "%sprotected:\n", ind.c_str());
            }
            if (!methods[i].isProtected && mode=="protected") {
                mode = "public";
                fprintf(hpp, "%spublic:\n", ind.c_str());
            }
            methods[i].write(hpp, cpp, context, indent + 4);            
        }
        fprintf(hpp, "%s};\n", ind.c_str());
        
    }
};

}
#endif

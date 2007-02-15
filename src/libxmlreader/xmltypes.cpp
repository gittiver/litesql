#include "litesql/xmltypes.hpp"
#include "stringutils.hpp"
#include "litesql/split.hpp"
using namespace litesql;
namespace std {
    template <>
    struct less<xml::Relate*> {
        bool operator()(xml::Relate const* r1, xml::Relate const* r2) {
            if (!r1)
                return true;
            if (!r2)
                return false;
                
            return r1->objectName < r2->objectName;
        }
    };
}

namespace xml {
    using namespace std;

    Field::Field(const Position& p,
            const std::string& n,
            const std::string& t,
            const std::string& d,
            bool i,
            bool u) 
        : XML(p), name(n), fieldTypeName(capitalize(n)), typeName(t),
          type(NULL), default_(d), indexed(i), unique(u) {}


    bool Field::hasQuotedValues() const {
        if (!type)
            return true;
        return type->quotedValue;
    }
    string Field::getQuotedDefaultValue() const {
        if (hasQuotedValues())
            return "\"" + default_ + "\"";
        if (!default_.empty())
            return "0";
        return default_;
    }

    string Field::getClass(const string& target) const {
        for (size_t i = 0; i < type->represents.size(); i++) {
            Represent* r = type->represents[i];
            if (r->target == target)
                return r->as;
        }
        return type->name;
    }

    int Object::getLastFieldOffset() const {
        if (!parentObject)
            return fields.size();
        return parentObject->getLastFieldOffset() + fields.size();
    }
    void Object::getAllFields(vector<Field*>& flds) const {
        if (parentObject)
            parentObject->getAllFields(flds);
        for (size_t i = 0; i < fields.size(); i++)
            flds.push_back(fields[i]);

    }

    void Object::getChildrenNames(Split& names) const {
        for (size_t i = 0; i < children.size(); i++) {
            names.push_back(children[i]->name);
            children[i]->getChildrenNames(names);
        }

    }
    Object* Object::getBaseObject() const {
        if (!parentObject)
            return const_cast<Object*>(this);
        return parentObject->getBaseObject();
    }

    string Object::getTable() const {
        return makeDbName(name + "_");        
    }
    string Object::getSequence() const {
        return makeDbName(name + "_seq");
    }

    Relate* Relate::clone() const {
        Relate* r = new Relate(pos, relation, objectName, interfaceName,
                 limit, unique, handle);

        r->paramPos = paramPos;
        r->sameObjectPos = sameObjectPos;
        r->object = object;
        r->interface = interface;
        r->fieldTypeName = fieldTypeName;
        r->fieldName = fieldName;
        return r;                               
    }

    std::string Relation::getTable() const {
        Split res;
        for (size_t i = 0; i < related.size(); i++)
            res.push_back(related[i]->objectName);
        res.push_back(id);

        return makeDbName(res.join("_"));

    }
    int Relation::countTypes(const std::string& obj) const {
        int res = 0;
        for (size_t i = 0; i < related.size(); i++)
            if (related[i]->objectName == name)
                res++;
        return res;

    }
    int Relation::maxSameTypes() const {
        map<string, int> names;
        int max = 0;
        for (size_t i = 0; i < related.size(); i++) {
            if (names.find(related[i]->objectName) == names.end())
                names[related[i]->objectName] = 0; 
            int value = ++names[related[i]->objectName];
            if (value > max)
                max = value;
        }
        return max;
    }

    Relation* Relation::clone() const {
        Relation* r = new Relation(pos, id, name);

        for (size_t i = 0; i < related.size(); i++)
            r->related.push_back(related[i]->clone());

        r->fields = fields;
        r->indices = indices;
        r->options = options;
        r->checks = checks;

        return r;
    }

    void Relation::sortRelated() {
        sort(related.begin(), related.end(), less<Relate*>());

        bool same = maxSameTypes() > 1;

        map<string, int> nameCounter;

        for (size_t i = 0; i < related.size(); i++) {
            Relate& relate = *related[i];
            string num;

            if (same)
                num = litesql::toString(i + 1);

            if (nameCounter.find(relate.objectName) == nameCounter.end())
                nameCounter[relate.objectName] = 1;
            else
                nameCounter[relate.objectName] += 1;

            relate.fieldTypeName = relate.objectName + num;
            relate.fieldName = relate.objectName + litesql::toString(i + 1);
            relate.paramPos = i;
            relate.sameObjectPos = nameCounter[relate.objectName];
        }

    }

    void Database::initBaseTypes() {

        Position p("<internal>", 0);
        Type* intType    = new Type(p, "integer");
        Type* floatType  = new Type(p, "float");
        Type* doubleType = new Type(p, "double");
        Type* stringType = new Type(p, "string");
        Type* dateType   = new Type(p, "date");
        Type* timeType   = new Type(p, "time");
        Type* dateTimeType = new Type(p, "datetime");
        Type* boolType = new Type(p, "boolean");

        intType->stores.push_back(new Store(p, "INTEGER", ""));
        floatType->stores.push_back(new Store(p, "FLOAT", ""));
        doubleType->stores.push_back(new Store(p, "FLOAT", ""));
        stringType->stores.push_back(new Store(p, "TEXT", ""));
        dateType->stores.push_back(new Store(p, "DATE", ""));
        timeType->stores.push_back(new Store(p, "TIME", ""));
        dateTimeType->stores.push_back(new Store(p, "DATETIME", ""));
        boolType->stores.push_back(new Store(p, "SMALLINT", ""));

        intType->represents.push_back(new Represent(p, "int", "c++"));
        floatType->represents.push_back(new Represent(p, "float", "c++"));
        doubleType->represents.push_back(new Represent(p, "double", "c++"));
        stringType->represents.push_back(new Represent(p, "std::string", 
                    "c++"));
        dateType->represents.push_back(new Represent(p, "litesql::Date","c++"));
        timeType->represents.push_back(new Represent(p,"litesql::Time", "c++"));
        dateTimeType->represents.push_back(new Represent(p,"litesql::DateTime", 
                    "c++"));

        intType->represents.push_back(new Represent(p, "int", "python"));
        floatType->represents.push_back(new Represent(p, "float", "python"));
        doubleType->represents.push_back(new Represent(p, "float", "python"));
        stringType->represents.push_back(new Represent(p, "str", "python"));
        dateType->represents.push_back(new Represent(p, "litesql.Date",
                    "python"));
        timeType->represents.push_back(new Represent(p,"litesql.Time", 
                    "python"));
        dateTimeType->represents.push_back(new Represent(p,"litesql.DateTime", 
                    "python"));
        types.push_back(intType);
        types.push_back(floatType);
        types.push_back(doubleType);
        types.push_back(stringType);
        types.push_back(dateType);
        types.push_back(timeType);
        types.push_back(dateTimeType);
        types.push_back(boolType);
    }

}

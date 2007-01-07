#include "xmltypes.hpp"

namespace xml {
    using namespace std;
    string Field::getSQLType() const {
        if (!type)
            return "";

        return type->sqlType;
    }
    string Field::getClass() const {
        if (!type)
            return "";

        return type->class_;
    }

    bool Field::hasQuotedValues() const {
        if (!type)
            return true;
        return type->quotedValues;
    }
    string Field::getQuotedDefaultValue() const {
        if (hasQuotedValues())
            return "\"" + default_ + "\"";
        if (!default_.empty())
            return "0";
        return default_;
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
    const Object* Object::getBaseObject() const {
        if (!parentObject)
            return this;
        return parentObject->getBaseObject();
    }

    string Object::getTable() const {
        return makeDBName(name + "_");        
    }
    string Object::getSequence() const {
        return makeDBName(name + "_seq");
    }
    std::string Relation::getTable() const {
        Split res;
        for (size_t i = 0; i < related.size(); i++)
            res.push_back(related[i]->objectName);
        res.push_back(id);

        return makeDBName(res.join("_"));

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

}

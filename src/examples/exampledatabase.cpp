#include "exampledatabase.hpp"
namespace example {
using namespace litesql;
PersonPersonRelationMother::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : person2(PersonPersonRelationMother::person2), person1(PersonPersonRelationMother::person1) {
    switch(rec.size()) {
    case 2:
        person2 = rec[1];
    case 1:
        person1 = rec[0];
    }
}
const std::string PersonPersonRelationMother::table("Person_Person_Mother");
const litesql::FieldType PersonPersonRelationMother::person1("Person1","INTEGER",table);
const litesql::FieldType PersonPersonRelationMother::person2("Person2","INTEGER",table);
void PersonPersonRelationMother::link(const litesql::Database& db, const Person& o0, const Person& o1) {
    Record values;
    Split fields;
    fields.push_back(person1.name());
    values.push_back(o0.id);
    fields.push_back(person2.name());
    values.push_back(o1.id);
    db.insert(table, values, fields);
}
void PersonPersonRelationMother::unlink(const litesql::Database& db, const Person& o0, const Person& o1) {
    db.delete_(table, (person1 == o0.id && person2 == o1.id));
}
void PersonPersonRelationMother::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table, expr);
}
litesql::DataSource<PersonPersonRelationMother::Row> PersonPersonRelationMother::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(person1.fullName());
    sel.result(person2.fullName());
    sel.source(table);
    sel.where(expr);
    return DataSource<PersonPersonRelationMother::Row>(db, sel);
}
litesql::DataSource<Person> PersonPersonRelationMother::getPerson1(const litesql::Database& db, const litesql::Expr& srcExpr, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.source(table);
    sel.result(person1.fullName());
    sel.where(srcExpr);
    return DataSource<Person>(db, Person::id_.in(sel) && expr);
}
litesql::DataSource<Person> PersonPersonRelationMother::getPerson2(const litesql::Database& db, const litesql::Expr& srcExpr, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.source(table);
    sel.result(person2.fullName());
    sel.where(srcExpr);
    return DataSource<Person>(db, Person::id_.in(sel) && expr);
}
PersonPersonRelationFather::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : person2(PersonPersonRelationFather::person2), person1(PersonPersonRelationFather::person1) {
    switch(rec.size()) {
    case 2:
        person2 = rec[1];
    case 1:
        person1 = rec[0];
    }
}
const std::string PersonPersonRelationFather::table("Person_Person_Father");
const litesql::FieldType PersonPersonRelationFather::person1("Person1","INTEGER",table);
const litesql::FieldType PersonPersonRelationFather::person2("Person2","INTEGER",table);
void PersonPersonRelationFather::link(const litesql::Database& db, const Person& o0, const Person& o1) {
    Record values;
    Split fields;
    fields.push_back(person1.name());
    values.push_back(o0.id);
    fields.push_back(person2.name());
    values.push_back(o1.id);
    db.insert(table, values, fields);
}
void PersonPersonRelationFather::unlink(const litesql::Database& db, const Person& o0, const Person& o1) {
    db.delete_(table, (person1 == o0.id && person2 == o1.id));
}
void PersonPersonRelationFather::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table, expr);
}
litesql::DataSource<PersonPersonRelationFather::Row> PersonPersonRelationFather::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(person1.fullName());
    sel.result(person2.fullName());
    sel.source(table);
    sel.where(expr);
    return DataSource<PersonPersonRelationFather::Row>(db, sel);
}
litesql::DataSource<Person> PersonPersonRelationFather::getPerson1(const litesql::Database& db, const litesql::Expr& srcExpr, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.source(table);
    sel.result(person1.fullName());
    sel.where(srcExpr);
    return DataSource<Person>(db, Person::id_.in(sel) && expr);
}
litesql::DataSource<Person> PersonPersonRelationFather::getPerson2(const litesql::Database& db, const litesql::Expr& srcExpr, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.source(table);
    sel.result(person2.fullName());
    sel.where(srcExpr);
    return DataSource<Person>(db, Person::id_.in(sel) && expr);
}
PersonPersonRelationSiblings::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : person2(PersonPersonRelationSiblings::person2), person1(PersonPersonRelationSiblings::person1) {
    switch(rec.size()) {
    case 2:
        person2 = rec[1];
    case 1:
        person1 = rec[0];
    }
}
const std::string PersonPersonRelationSiblings::table("Person_Person_Siblings");
const litesql::FieldType PersonPersonRelationSiblings::person1("Person1","INTEGER",table);
const litesql::FieldType PersonPersonRelationSiblings::person2("Person2","INTEGER",table);
void PersonPersonRelationSiblings::link(const litesql::Database& db, const Person& o0, const Person& o1) {
    Record values;
    Split fields;
    fields.push_back(person1.name());
    values.push_back(o0.id);
    fields.push_back(person2.name());
    values.push_back(o1.id);
    db.insert(table, values, fields);
    fields.clear();
    values.clear();
    fields.push_back(person1.name());
    values.push_back(o1.id);
    fields.push_back(person2.name());
    values.push_back(o0.id);
    db.insert(table, values, fields);
}
void PersonPersonRelationSiblings::unlink(const litesql::Database& db, const Person& o0, const Person& o1) {
    db.delete_(table, (person1 == o0.id && person2 == o1.id));
    db.delete_(table, (person1 == o1.id && person2 == o0.id));
}
void PersonPersonRelationSiblings::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table, expr);
}
litesql::DataSource<PersonPersonRelationSiblings::Row> PersonPersonRelationSiblings::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(person1.fullName());
    sel.result(person2.fullName());
    sel.source(table);
    sel.where(expr);
    return DataSource<PersonPersonRelationSiblings::Row>(db, sel);
}
litesql::DataSource<Person> PersonPersonRelationSiblings::getPerson1(const litesql::Database& db, const litesql::Expr& srcExpr, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.source(table);
    sel.result(person1.fullName());
    sel.where(srcExpr);
    return DataSource<Person>(db, Person::id_.in(sel) && expr);
}
litesql::DataSource<Person> PersonPersonRelationSiblings::getPerson2(const litesql::Database& db, const litesql::Expr& srcExpr, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.source(table);
    sel.result(person2.fullName());
    sel.where(srcExpr);
    return DataSource<Person>(db, Person::id_.in(sel) && expr);
}
PersonPersonRelationChildren::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : person2(PersonPersonRelationChildren::person2), person1(PersonPersonRelationChildren::person1) {
    switch(rec.size()) {
    case 2:
        person2 = rec[1];
    case 1:
        person1 = rec[0];
    }
}
const std::string PersonPersonRelationChildren::table("Person_Person_Children");
const litesql::FieldType PersonPersonRelationChildren::person1("Person1","INTEGER",table);
const litesql::FieldType PersonPersonRelationChildren::person2("Person2","INTEGER",table);
void PersonPersonRelationChildren::link(const litesql::Database& db, const Person& o0, const Person& o1) {
    Record values;
    Split fields;
    fields.push_back(person1.name());
    values.push_back(o0.id);
    fields.push_back(person2.name());
    values.push_back(o1.id);
    db.insert(table, values, fields);
}
void PersonPersonRelationChildren::unlink(const litesql::Database& db, const Person& o0, const Person& o1) {
    db.delete_(table, (person1 == o0.id && person2 == o1.id));
}
void PersonPersonRelationChildren::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table, expr);
}
litesql::DataSource<PersonPersonRelationChildren::Row> PersonPersonRelationChildren::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(person1.fullName());
    sel.result(person2.fullName());
    sel.source(table);
    sel.where(expr);
    return DataSource<PersonPersonRelationChildren::Row>(db, sel);
}
litesql::DataSource<Person> PersonPersonRelationChildren::getPerson1(const litesql::Database& db, const litesql::Expr& srcExpr, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.source(table);
    sel.result(person1.fullName());
    sel.where(srcExpr);
    return DataSource<Person>(db, Person::id_.in(sel) && expr);
}
litesql::DataSource<Person> PersonPersonRelationChildren::getPerson2(const litesql::Database& db, const litesql::Expr& srcExpr, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.source(table);
    sel.result(person2.fullName());
    sel.where(srcExpr);
    return DataSource<Person>(db, Person::id_.in(sel) && expr);
}
const int Person::Sex::Male(0);
const int Person::Sex::Female(1);
Person::MotherHandle::MotherHandle(const Person& owner)
         : litesql::RelationHandle<Person>(owner) {
}
void Person::MotherHandle::link(const Person& o0) {
    PersonPersonRelationMother::link(owner->getDatabase(), *owner, o0);
}
void Person::MotherHandle::unlink(const Person& o0) {
    PersonPersonRelationMother::unlink(owner->getDatabase(), *owner, o0);
}
void Person::MotherHandle::del(const litesql::Expr& expr) {
    PersonPersonRelationMother::del(owner->getDatabase(), expr);
}
litesql::DataSource<Person> Person::MotherHandle::get(const litesql::Expr& expr) {
    return PersonPersonRelationMother::getPerson2(owner->getDatabase(), (PersonPersonRelationMother::person1 == owner->id), expr);
}
litesql::DataSource<PersonPersonRelationMother::Row> Person::MotherHandle::getRows(const litesql::Expr& expr) {
    return PersonPersonRelationMother::getRows(owner->getDatabase(), expr && (PersonPersonRelationMother::person1 == owner->id));
}
Person::FatherHandle::FatherHandle(const Person& owner)
         : litesql::RelationHandle<Person>(owner) {
}
void Person::FatherHandle::link(const Person& o0) {
    PersonPersonRelationFather::link(owner->getDatabase(), *owner, o0);
}
void Person::FatherHandle::unlink(const Person& o0) {
    PersonPersonRelationFather::unlink(owner->getDatabase(), *owner, o0);
}
void Person::FatherHandle::del(const litesql::Expr& expr) {
    PersonPersonRelationFather::del(owner->getDatabase(), expr);
}
litesql::DataSource<Person> Person::FatherHandle::get(const litesql::Expr& expr) {
    return PersonPersonRelationFather::getPerson2(owner->getDatabase(), (PersonPersonRelationFather::person1 == owner->id), expr);
}
litesql::DataSource<PersonPersonRelationFather::Row> Person::FatherHandle::getRows(const litesql::Expr& expr) {
    return PersonPersonRelationFather::getRows(owner->getDatabase(), expr && (PersonPersonRelationFather::person1 == owner->id));
}
Person::SiblingsHandle::SiblingsHandle(const Person& owner)
         : litesql::RelationHandle<Person>(owner) {
}
void Person::SiblingsHandle::link(const Person& o0) {
    PersonPersonRelationSiblings::link(owner->getDatabase(), *owner, o0);
}
void Person::SiblingsHandle::unlink(const Person& o0) {
    PersonPersonRelationSiblings::unlink(owner->getDatabase(), *owner, o0);
}
void Person::SiblingsHandle::del(const litesql::Expr& expr) {
    PersonPersonRelationSiblings::del(owner->getDatabase(), expr);
}
litesql::DataSource<Person> Person::SiblingsHandle::get(const litesql::Expr& expr) {
    return PersonPersonRelationSiblings::getPerson2(owner->getDatabase(), (PersonPersonRelationSiblings::person1 == owner->id), expr);
}
litesql::DataSource<PersonPersonRelationSiblings::Row> Person::SiblingsHandle::getRows(const litesql::Expr& expr) {
    return PersonPersonRelationSiblings::getRows(owner->getDatabase(), expr && (PersonPersonRelationSiblings::person1 == owner->id));
}
Person::ChildrenHandle::ChildrenHandle(const Person& owner)
         : litesql::RelationHandle<Person>(owner) {
}
void Person::ChildrenHandle::link(const Person& o0) {
    PersonPersonRelationChildren::link(owner->getDatabase(), *owner, o0);
}
void Person::ChildrenHandle::unlink(const Person& o0) {
    PersonPersonRelationChildren::unlink(owner->getDatabase(), *owner, o0);
}
void Person::ChildrenHandle::del(const litesql::Expr& expr) {
    PersonPersonRelationChildren::del(owner->getDatabase(), expr);
}
litesql::DataSource<Person> Person::ChildrenHandle::get(const litesql::Expr& expr) {
    return PersonPersonRelationChildren::getPerson2(owner->getDatabase(), (PersonPersonRelationChildren::person1 == owner->id), expr);
}
litesql::DataSource<PersonPersonRelationChildren::Row> Person::ChildrenHandle::getRows(const litesql::Expr& expr) {
    return PersonPersonRelationChildren::getRows(owner->getDatabase(), expr && (PersonPersonRelationChildren::person1 == owner->id));
}
const std::string Person::type__("Person");
const std::string Person::table__("Person_");
const std::string Person::sequence__("Person_seq");
const litesql::FieldType Person::id_("id_","INTEGER",table__);
const litesql::FieldType Person::type_("type_","TEXT",table__);
const litesql::FieldType Person::name_("name_","TEXT",table__);
const litesql::FieldType Person::age_("age_","INTEGER",table__);
const litesql::FieldType Person::sex_("sex_","INTEGER",table__);
void Person::defaults() {
    id = 0;
    age = 15;
    sex = 0;
}
Person::Person(const litesql::Database& db)
     : litesql::Persistent(db), id(id_), type(type_), name(name_), age(age_), sex(sex_) {
    defaults();
}
Person::Person(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db), id(id_), type(type_), name(name_), age(age_), sex(sex_) {
    defaults();
    switch(rec.size()) {
    case 5: sex = rec[4];
    case 4: age = rec[3];
    case 3: name = rec[2];
    case 2: type = rec[1];
    case 1: id = rec[0];
    }
}
Person::Person(const Person& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), name(obj.name), age(obj.age), sex(obj.sex) {
}
const Person& Person::operator=(const Person& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        name = obj.name;
        age = obj.age;
        sex = obj.sex;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
Person::MotherHandle Person::mother() {
    return Person::MotherHandle(*this);
}
Person::FatherHandle Person::father() {
    return Person::FatherHandle(*this);
}
Person::SiblingsHandle Person::siblings() {
    return Person::SiblingsHandle(*this);
}
Person::ChildrenHandle Person::children() {
    return Person::ChildrenHandle(*this);
}
std::string Person::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
    tables.push_back(table__);
    litesql::Record fields;
    litesql::Record values;
    fields.push_back(id.name());
    values.push_back(id);
    id.setModified(false);
    fields.push_back(type.name());
    values.push_back(type);
    type.setModified(false);
    fields.push_back(name.name());
    values.push_back(name);
    name.setModified(false);
    fields.push_back(age.name());
    values.push_back(age);
    age.setModified(false);
    fields.push_back(sex.name());
    values.push_back(sex);
    sex.setModified(false);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void Person::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void Person::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
    updateField(updates, table__, name);
    updateField(updates, table__, age);
    updateField(updates, table__, sex);
}
void Person::addIDUpdates(Updates& updates) {
}
void Person::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(id_);
    ftypes.push_back(type_);
    ftypes.push_back(name_);
    ftypes.push_back(age_);
    ftypes.push_back(sex_);
}
void Person::delRecord() {
    deleteFromTable(table__, id);
}
void Person::delRelations() {
    PersonPersonRelationMother::del(*db, (PersonPersonRelationMother::person1 == id) || (PersonPersonRelationMother::person2 == id));
    PersonPersonRelationFather::del(*db, (PersonPersonRelationFather::person1 == id) || (PersonPersonRelationFather::person2 == id));
    PersonPersonRelationSiblings::del(*db, (PersonPersonRelationSiblings::person1 == id) || (PersonPersonRelationSiblings::person2 == id));
    PersonPersonRelationChildren::del(*db, (PersonPersonRelationChildren::person1 == id) || (PersonPersonRelationChildren::person2 == id));
}
void Person::update() {
    if (!inDatabase) {
        create();
        return;
    }
    Updates updates;
    addUpdates(updates);
    if (id != oldKey) {
        if (!typeIsCorrect()) 
            upcastCopy()->addIDUpdates(updates);
    }
    litesql::Persistent::update(updates);
    oldKey = id;
}
void Person::del() {
    if (typeIsCorrect() == false) {
        std::auto_ptr<Person> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool Person::typeIsCorrect() {
    return type == type__;
}
std::auto_ptr<Person> Person::upcast() {
    return auto_ptr<Person>(new Person(*this));
}
std::auto_ptr<Person> Person::upcastCopy() {
    Person* np = NULL;
    np->id = id;
    np->type = type;
    np->name = name;
    np->age = age;
    np->sex = sex;
    np->inDatabase = inDatabase;
    if (!np)
        np = new Person(*this);
    return auto_ptr<Person>(np);
}
std::ostream & operator<<(std::ostream& os, Person o) {
    os << "-------------------------------------"<< std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.name.name() << " = " << o.name << std::endl;
    os << o.age.name() << " = " << o.age << std::endl;
    os << o.sex.name() << " = " << o.sex << std::endl;
    os << "-------------------------------------"<< std::endl;
    return os;
}
ExampleDatabase::ExampleDatabase(std::string backendType, std::string connInfo)
     : litesql::Database(backendType, connInfo) {
}
std::vector<litesql::Database::SchemaItem> ExampleDatabase::getSchema() const {
    vector<Database::SchemaItem> res;
    res.push_back(Database::SchemaItem("schema","table","CREATE TABLE schema (name TEXT, type TEXT, sql TEXT);"));
    res.push_back(Database::SchemaItem(Person::table__,"table","CREATE TABLE " + Person::table__ + " (" + Person::id_.name() + " " + backend->getRowIDType() + "," + Person::type_.name() + " " + Person::type_.type() + "," + Person::name_.name() + " " + Person::name_.type() + "," + Person::age_.name() + " " + Person::age_.type() + "," + Person::sex_.name() + " " + Person::sex_.type() + ")"));
    if (backend->supportsSequences())
    res.push_back(Database::SchemaItem(Person::sequence__,"sequence","CREATE SEQUENCE " + Person::sequence__ + " START 1 INCREMENT 1"));
    res.push_back(Database::SchemaItem(PersonPersonRelationMother::table,"table","CREATE TABLE " + PersonPersonRelationMother::table + " (" + PersonPersonRelationMother::person1.name() + " " + PersonPersonRelationMother::person1.type() + " UNIQUE"+ "," + PersonPersonRelationMother::person2.name() + " " + PersonPersonRelationMother::person2.type() + ")"));
    res.push_back(Database::SchemaItem(PersonPersonRelationFather::table,"table","CREATE TABLE " + PersonPersonRelationFather::table + " (" + PersonPersonRelationFather::person1.name() + " " + PersonPersonRelationFather::person1.type() + " UNIQUE"+ "," + PersonPersonRelationFather::person2.name() + " " + PersonPersonRelationFather::person2.type() + ")"));
    res.push_back(Database::SchemaItem(PersonPersonRelationSiblings::table,"table","CREATE TABLE " + PersonPersonRelationSiblings::table + " (" + PersonPersonRelationSiblings::person1.name() + " " + PersonPersonRelationSiblings::person1.type()+ "," + PersonPersonRelationSiblings::person2.name() + " " + PersonPersonRelationSiblings::person2.type() + ")"));
    res.push_back(Database::SchemaItem(PersonPersonRelationChildren::table,"table","CREATE TABLE " + PersonPersonRelationChildren::table + " (" + PersonPersonRelationChildren::person1.name() + " " + PersonPersonRelationChildren::person1.type()+ "," + PersonPersonRelationChildren::person2.name() + " " + PersonPersonRelationChildren::person2.type() + ")"));
    return res;
}
}

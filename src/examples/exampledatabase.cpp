#include "exampledatabase.hpp"
namespace example {
using namespace litesql;
PersonPersonRelationMother::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : person2(PersonPersonRelationMother::person2_), person1(PersonPersonRelationMother::person1_) {
    switch(rec.size()) {
    case 2:
        person2 = rec[1];
    case 1:
        person1 = rec[0];
    }
}
const std::string PersonPersonRelationMother::table__("Person_Person_Mother");
const litesql::FieldType PersonPersonRelationMother::person1_("Person1","INTEGER",table__);
const litesql::FieldType PersonPersonRelationMother::person2_("Person2","INTEGER",table__);
void PersonPersonRelationMother::link(const litesql::Database& db, const Person& o0, const Person& o1) {
    Record values;
    Split fields;
    fields.push_back(person1_.name());
    values.push_back(o0.id);
    fields.push_back(person2_.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void PersonPersonRelationMother::unlink(const litesql::Database& db, const Person& o0, const Person& o1) {
    db.delete_(table__, (person1_ == o0.id && person2_ == o1.id));
}
void PersonPersonRelationMother::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<PersonPersonRelationMother::Row> PersonPersonRelationMother::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(person1_.fullName());
    sel.result(person2_.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<PersonPersonRelationMother::Row>(db, sel);
}
litesql::DataSource<Person> PersonPersonRelationMother::getPerson1(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(person1_.fullName());
    sel.where(srcExpr);
    return DataSource<Person>(db, Person::Id.in(sel) && expr);
}
litesql::DataSource<Person> PersonPersonRelationMother::getPerson2(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(person2_.fullName());
    sel.where(srcExpr);
    return DataSource<Person>(db, Person::Id.in(sel) && expr);
}
PersonPersonRelationFather::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : person2(PersonPersonRelationFather::person2_), person1(PersonPersonRelationFather::person1_) {
    switch(rec.size()) {
    case 2:
        person2 = rec[1];
    case 1:
        person1 = rec[0];
    }
}
const std::string PersonPersonRelationFather::table__("Person_Person_Father");
const litesql::FieldType PersonPersonRelationFather::person1_("Person1","INTEGER",table__);
const litesql::FieldType PersonPersonRelationFather::person2_("Person2","INTEGER",table__);
void PersonPersonRelationFather::link(const litesql::Database& db, const Person& o0, const Person& o1) {
    Record values;
    Split fields;
    fields.push_back(person1_.name());
    values.push_back(o0.id);
    fields.push_back(person2_.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void PersonPersonRelationFather::unlink(const litesql::Database& db, const Person& o0, const Person& o1) {
    db.delete_(table__, (person1_ == o0.id && person2_ == o1.id));
}
void PersonPersonRelationFather::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<PersonPersonRelationFather::Row> PersonPersonRelationFather::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(person1_.fullName());
    sel.result(person2_.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<PersonPersonRelationFather::Row>(db, sel);
}
litesql::DataSource<Person> PersonPersonRelationFather::getPerson1(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(person1_.fullName());
    sel.where(srcExpr);
    return DataSource<Person>(db, Person::Id.in(sel) && expr);
}
litesql::DataSource<Person> PersonPersonRelationFather::getPerson2(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(person2_.fullName());
    sel.where(srcExpr);
    return DataSource<Person>(db, Person::Id.in(sel) && expr);
}
PersonPersonRelationSiblings::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : person2(PersonPersonRelationSiblings::person2_), person1(PersonPersonRelationSiblings::person1_) {
    switch(rec.size()) {
    case 2:
        person2 = rec[1];
    case 1:
        person1 = rec[0];
    }
}
const std::string PersonPersonRelationSiblings::table__("Person_Person_Siblings");
const litesql::FieldType PersonPersonRelationSiblings::person1_("Person1","INTEGER",table__);
const litesql::FieldType PersonPersonRelationSiblings::person2_("Person2","INTEGER",table__);
void PersonPersonRelationSiblings::link(const litesql::Database& db, const Person& o0, const Person& o1) {
    Record values;
    Split fields;
    fields.push_back(person1_.name());
    values.push_back(o0.id);
    fields.push_back(person2_.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
    fields.clear();
    values.clear();
    fields.push_back(person1_.name());
    values.push_back(o1.id);
    fields.push_back(person2_.name());
    values.push_back(o0.id);
    db.insert(table__, values, fields);
}
void PersonPersonRelationSiblings::unlink(const litesql::Database& db, const Person& o0, const Person& o1) {
    db.delete_(table__, (person1_ == o0.id && person2_ == o1.id));
    db.delete_(table__, (person1_ == o1.id && person2_ == o0.id));
}
void PersonPersonRelationSiblings::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<PersonPersonRelationSiblings::Row> PersonPersonRelationSiblings::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(person1_.fullName());
    sel.result(person2_.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<PersonPersonRelationSiblings::Row>(db, sel);
}
litesql::DataSource<Person> PersonPersonRelationSiblings::getPerson1(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(person1_.fullName());
    sel.where(srcExpr);
    return DataSource<Person>(db, Person::Id.in(sel) && expr);
}
litesql::DataSource<Person> PersonPersonRelationSiblings::getPerson2(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(person2_.fullName());
    sel.where(srcExpr);
    return DataSource<Person>(db, Person::Id.in(sel) && expr);
}
PersonPersonRelationChildren::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : person2(PersonPersonRelationChildren::person2_), person1(PersonPersonRelationChildren::person1_) {
    switch(rec.size()) {
    case 2:
        person2 = rec[1];
    case 1:
        person1 = rec[0];
    }
}
const std::string PersonPersonRelationChildren::table__("Person_Person_Children");
const litesql::FieldType PersonPersonRelationChildren::person1_("Person1","INTEGER",table__);
const litesql::FieldType PersonPersonRelationChildren::person2_("Person2","INTEGER",table__);
void PersonPersonRelationChildren::link(const litesql::Database& db, const Person& o0, const Person& o1) {
    Record values;
    Split fields;
    fields.push_back(person1_.name());
    values.push_back(o0.id);
    fields.push_back(person2_.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void PersonPersonRelationChildren::unlink(const litesql::Database& db, const Person& o0, const Person& o1) {
    db.delete_(table__, (person1_ == o0.id && person2_ == o1.id));
}
void PersonPersonRelationChildren::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<PersonPersonRelationChildren::Row> PersonPersonRelationChildren::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(person1_.fullName());
    sel.result(person2_.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<PersonPersonRelationChildren::Row>(db, sel);
}
litesql::DataSource<Person> PersonPersonRelationChildren::getPerson1(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(person1_.fullName());
    sel.where(srcExpr);
    return DataSource<Person>(db, Person::Id.in(sel) && expr);
}
litesql::DataSource<Person> PersonPersonRelationChildren::getPerson2(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(person2_.fullName());
    sel.where(srcExpr);
    return DataSource<Person>(db, Person::Id.in(sel) && expr);
}
const int Person::SexType::Male(0);
const int Person::SexType::Female(1);
Person::SexType::SexType(const std::string& n, const std::string& t, const std::string& tbl, const litesql::FieldType::Values& vals)
         : litesql::FieldType(n,t,tbl,vals) {
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
litesql::DataSource<Person> Person::MotherHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return PersonPersonRelationMother::getPerson2(owner->getDatabase(), expr, (PersonPersonRelationMother::person1_ == owner->id) && srcExpr);
}
litesql::DataSource<PersonPersonRelationMother::Row> Person::MotherHandle::getRows(const litesql::Expr& expr) {
    return PersonPersonRelationMother::getRows(owner->getDatabase(), expr && (PersonPersonRelationMother::person1_ == owner->id));
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
litesql::DataSource<Person> Person::FatherHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return PersonPersonRelationFather::getPerson2(owner->getDatabase(), expr, (PersonPersonRelationFather::person1_ == owner->id) && srcExpr);
}
litesql::DataSource<PersonPersonRelationFather::Row> Person::FatherHandle::getRows(const litesql::Expr& expr) {
    return PersonPersonRelationFather::getRows(owner->getDatabase(), expr && (PersonPersonRelationFather::person1_ == owner->id));
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
litesql::DataSource<Person> Person::SiblingsHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return PersonPersonRelationSiblings::getPerson2(owner->getDatabase(), expr, (PersonPersonRelationSiblings::person1_ == owner->id) && srcExpr);
}
litesql::DataSource<PersonPersonRelationSiblings::Row> Person::SiblingsHandle::getRows(const litesql::Expr& expr) {
    return PersonPersonRelationSiblings::getRows(owner->getDatabase(), expr && (PersonPersonRelationSiblings::person1_ == owner->id));
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
litesql::DataSource<Person> Person::ChildrenHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return PersonPersonRelationChildren::getPerson2(owner->getDatabase(), expr, (PersonPersonRelationChildren::person1_ == owner->id) && srcExpr);
}
litesql::DataSource<PersonPersonRelationChildren::Row> Person::ChildrenHandle::getRows(const litesql::Expr& expr) {
    return PersonPersonRelationChildren::getRows(owner->getDatabase(), expr && (PersonPersonRelationChildren::person1_ == owner->id));
}
const std::string Person::type__("Person");
const std::string Person::table__("Person_");
const std::string Person::sequence__("Person_seq");
const litesql::FieldType Person::Id("id_","INTEGER",table__);
const litesql::FieldType Person::Type("type_","TEXT",table__);
const litesql::FieldType Person::Name("name_","TEXT",table__);
const litesql::FieldType Person::Age("age_","INTEGER",table__);
std::vector < std::pair< std::string, std::string > > Person::sex_values;
Person::SexType Person::Sex("sex_","INTEGER",table__,sex_values);
void Person::initValues() {
    sex_values.clear();
    sex_values.push_back(make_pair("Male","0"));
    sex_values.push_back(make_pair("Female","1"));
}
void Person::defaults() {
    id = 0;
    age = 15;
    sex = 0;
}
Person::Person(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), name(Name), age(Age), sex(Sex) {
    defaults();
}
Person::Person(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), name(Name), age(Age), sex(Sex) {
    defaults();
    size_t size = (rec.size() > 5) ? 5 : rec.size();
    switch(size) {
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
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(Name);
    ftypes.push_back(Age);
    ftypes.push_back(Sex);
}
void Person::delRecord() {
    deleteFromTable(table__, id);
}
void Person::delRelations() {
    PersonPersonRelationMother::del(*db, (PersonPersonRelationMother::person1_ == id) || (PersonPersonRelationMother::person2_ == id));
    PersonPersonRelationFather::del(*db, (PersonPersonRelationFather::person1_ == id) || (PersonPersonRelationFather::person2_ == id));
    PersonPersonRelationSiblings::del(*db, (PersonPersonRelationSiblings::person1_ == id) || (PersonPersonRelationSiblings::person2_ == id));
    PersonPersonRelationChildren::del(*db, (PersonPersonRelationChildren::person1_ == id) || (PersonPersonRelationChildren::person2_ == id));
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
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.name.name() << " = " << o.name << std::endl;
    os << o.age.name() << " = " << o.age << std::endl;
    os << o.sex.name() << " = " << o.sex << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
ExampleDatabase::ExampleDatabase(std::string backendType, std::string connInfo)
     : litesql::Database(backendType, connInfo) {
    initialize();
}
std::vector<litesql::Database::SchemaItem> ExampleDatabase::getSchema() const {
    vector<Database::SchemaItem> res;
    res.push_back(Database::SchemaItem("schema","table","CREATE TABLE schema (name TEXT, type TEXT, sql TEXT);"));
    res.push_back(Database::SchemaItem(Person::table__,"table","CREATE TABLE " + Person::table__ + " (" + Person::Id.name() + " " + backend->getRowIDType() + "," + Person::Type.name() + " " + Person::Type.type() + "," + Person::Name.name() + " " + Person::Name.type() + "," + Person::Age.name() + " " + Person::Age.type() + "," + Person::Sex.name() + " " + Person::Sex.type() + ")"));
    if (backend->supportsSequences())
    res.push_back(Database::SchemaItem(Person::sequence__,"sequence","CREATE SEQUENCE " + Person::sequence__ + " START 1 INCREMENT 1"));
    res.push_back(Database::SchemaItem(PersonPersonRelationMother::table__,"table","CREATE TABLE " + PersonPersonRelationMother::table__ + " (" + PersonPersonRelationMother::person1_.name() + " " + PersonPersonRelationMother::person1_.type() + " UNIQUE"+ "," + PersonPersonRelationMother::person2_.name() + " " + PersonPersonRelationMother::person2_.type() + ")"));
    res.push_back(Database::SchemaItem("Person_Person_Mother_all_idx","index","CREATE INDEX Person_Person_Mother_all_idx ON " + PersonPersonRelationMother::table__ + " (" + PersonPersonRelationMother::person1_.name() + "," + PersonPersonRelationMother::person2_.name() + ")"));
    res.push_back(Database::SchemaItem("_44c3341c760706ead4261b63d7c097e4","index","CREATE INDEX _44c3341c760706ead4261b63d7c097e4 ON " + PersonPersonRelationMother::table__ + " (" + PersonPersonRelationMother::person1_.name() + ")"));
    res.push_back(Database::SchemaItem("_b52edb51f756af1fc3ca2899975840ba","index","CREATE INDEX _b52edb51f756af1fc3ca2899975840ba ON " + PersonPersonRelationMother::table__ + " (" + PersonPersonRelationMother::person2_.name() + ")"));
    res.push_back(Database::SchemaItem(PersonPersonRelationFather::table__,"table","CREATE TABLE " + PersonPersonRelationFather::table__ + " (" + PersonPersonRelationFather::person1_.name() + " " + PersonPersonRelationFather::person1_.type() + " UNIQUE"+ "," + PersonPersonRelationFather::person2_.name() + " " + PersonPersonRelationFather::person2_.type() + ")"));
    res.push_back(Database::SchemaItem("Person_Person_Father_all_idx","index","CREATE INDEX Person_Person_Father_all_idx ON " + PersonPersonRelationFather::table__ + " (" + PersonPersonRelationFather::person1_.name() + "," + PersonPersonRelationFather::person2_.name() + ")"));
    res.push_back(Database::SchemaItem("_228aa7019624272ae374453033b873a8","index","CREATE INDEX _228aa7019624272ae374453033b873a8 ON " + PersonPersonRelationFather::table__ + " (" + PersonPersonRelationFather::person1_.name() + ")"));
    res.push_back(Database::SchemaItem("_761687bdd9334f78054472392cc43263","index","CREATE INDEX _761687bdd9334f78054472392cc43263 ON " + PersonPersonRelationFather::table__ + " (" + PersonPersonRelationFather::person2_.name() + ")"));
    res.push_back(Database::SchemaItem(PersonPersonRelationSiblings::table__,"table","CREATE TABLE " + PersonPersonRelationSiblings::table__ + " (" + PersonPersonRelationSiblings::person1_.name() + " " + PersonPersonRelationSiblings::person1_.type()+ "," + PersonPersonRelationSiblings::person2_.name() + " " + PersonPersonRelationSiblings::person2_.type() + ")"));
    res.push_back(Database::SchemaItem("Person_Person_Siblings_all_idx","index","CREATE INDEX Person_Person_Siblings_all_idx ON " + PersonPersonRelationSiblings::table__ + " (" + PersonPersonRelationSiblings::person1_.name() + "," + PersonPersonRelationSiblings::person2_.name() + ")"));
    res.push_back(Database::SchemaItem("_665bc4736357284dcbd3285b596ca52a","index","CREATE INDEX _665bc4736357284dcbd3285b596ca52a ON " + PersonPersonRelationSiblings::table__ + " (" + PersonPersonRelationSiblings::person1_.name() + ")"));
    res.push_back(Database::SchemaItem("_4fd3d4d75c7cd3b2d4b889bcb9f1a164","index","CREATE INDEX _4fd3d4d75c7cd3b2d4b889bcb9f1a164 ON " + PersonPersonRelationSiblings::table__ + " (" + PersonPersonRelationSiblings::person2_.name() + ")"));
    res.push_back(Database::SchemaItem(PersonPersonRelationChildren::table__,"table","CREATE TABLE " + PersonPersonRelationChildren::table__ + " (" + PersonPersonRelationChildren::person1_.name() + " " + PersonPersonRelationChildren::person1_.type()+ "," + PersonPersonRelationChildren::person2_.name() + " " + PersonPersonRelationChildren::person2_.type() + ")"));
    res.push_back(Database::SchemaItem("Person_Person_Children_all_idx","index","CREATE INDEX Person_Person_Children_all_idx ON " + PersonPersonRelationChildren::table__ + " (" + PersonPersonRelationChildren::person1_.name() + "," + PersonPersonRelationChildren::person2_.name() + ")"));
    res.push_back(Database::SchemaItem("_05266866cc1464dd7088688615362c0f","index","CREATE INDEX _05266866cc1464dd7088688615362c0f ON " + PersonPersonRelationChildren::table__ + " (" + PersonPersonRelationChildren::person1_.name() + ")"));
    res.push_back(Database::SchemaItem("_a5a33f7281eb276234a26a4b83c413cb","index","CREATE INDEX _a5a33f7281eb276234a26a4b83c413cb ON " + PersonPersonRelationChildren::table__ + " (" + PersonPersonRelationChildren::person2_.name() + ")"));
    return res;
}
void ExampleDatabase::initialize() {
    static bool initialized = false;
    if (initialized)
        return;
    initialized = true;
    Person::initValues();
}
}

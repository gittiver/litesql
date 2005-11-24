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
    return DataSource<Person>(db, Person::id_.in(sel) && expr);
}
litesql::DataSource<Person> PersonPersonRelationMother::getPerson2(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(person2_.fullName());
    sel.where(srcExpr);
    return DataSource<Person>(db, Person::id_.in(sel) && expr);
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
    return DataSource<Person>(db, Person::id_.in(sel) && expr);
}
litesql::DataSource<Person> PersonPersonRelationFather::getPerson2(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(person2_.fullName());
    sel.where(srcExpr);
    return DataSource<Person>(db, Person::id_.in(sel) && expr);
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
    return DataSource<Person>(db, Person::id_.in(sel) && expr);
}
litesql::DataSource<Person> PersonPersonRelationSiblings::getPerson2(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(person2_.fullName());
    sel.where(srcExpr);
    return DataSource<Person>(db, Person::id_.in(sel) && expr);
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
    return DataSource<Person>(db, Person::id_.in(sel) && expr);
}
litesql::DataSource<Person> PersonPersonRelationChildren::getPerson2(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(person2_.fullName());
    sel.where(srcExpr);
    return DataSource<Person>(db, Person::id_.in(sel) && expr);
}
PersonRoleRelationRoles::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : role(PersonRoleRelationRoles::role_), person(PersonRoleRelationRoles::person_) {
    switch(rec.size()) {
    case 2:
        role = rec[1];
    case 1:
        person = rec[0];
    }
}
const std::string PersonRoleRelationRoles::table__("Person_Role_Roles");
const litesql::FieldType PersonRoleRelationRoles::person_("Person1","INTEGER",table__);
const litesql::FieldType PersonRoleRelationRoles::role_("Role2","INTEGER",table__);
void PersonRoleRelationRoles::link(const litesql::Database& db, const Person& o0, const Role& o1) {
    Record values;
    Split fields;
    fields.push_back(person_.name());
    values.push_back(o0.id);
    fields.push_back(role_.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void PersonRoleRelationRoles::unlink(const litesql::Database& db, const Person& o0, const Role& o1) {
    db.delete_(table__, (person_ == o0.id && role_ == o1.id));
}
void PersonRoleRelationRoles::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<PersonRoleRelationRoles::Row> PersonRoleRelationRoles::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(person_.fullName());
    sel.result(role_.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<PersonRoleRelationRoles::Row>(db, sel);
}
template <> litesql::DataSource<Person> PersonRoleRelationRoles::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(person_.fullName());
    sel.where(srcExpr);
    return DataSource<Person>(db, Person::id_.in(sel) && expr);
}
template <> litesql::DataSource<Role> PersonRoleRelationRoles::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(role_.fullName());
    sel.where(srcExpr);
    return DataSource<Role>(db, Role::id_.in(sel) && expr);
}
SchoolStudentRelation::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : student(SchoolStudentRelation::student_), school(SchoolStudentRelation::school_) {
    switch(rec.size()) {
    case 2:
        student = rec[1];
    case 1:
        school = rec[0];
    }
}
const std::string SchoolStudentRelation::table__("School_Student_");
const litesql::FieldType SchoolStudentRelation::school_("School1","INTEGER",table__);
const litesql::FieldType SchoolStudentRelation::student_("Student2","INTEGER",table__);
void SchoolStudentRelation::link(const litesql::Database& db, const School& o0, const Student& o1) {
    Record values;
    Split fields;
    fields.push_back(school_.name());
    values.push_back(o0.id);
    fields.push_back(student_.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void SchoolStudentRelation::unlink(const litesql::Database& db, const School& o0, const Student& o1) {
    db.delete_(table__, (school_ == o0.id && student_ == o1.id));
}
void SchoolStudentRelation::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<SchoolStudentRelation::Row> SchoolStudentRelation::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(school_.fullName());
    sel.result(student_.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<SchoolStudentRelation::Row>(db, sel);
}
template <> litesql::DataSource<School> SchoolStudentRelation::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(school_.fullName());
    sel.where(srcExpr);
    return DataSource<School>(db, School::id_.in(sel) && expr);
}
template <> litesql::DataSource<Student> SchoolStudentRelation::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(student_.fullName());
    sel.where(srcExpr);
    return DataSource<Student>(db, Student::id_.in(sel) && expr);
}
EmployeeOfficeRelation::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : office(EmployeeOfficeRelation::office_), employee(EmployeeOfficeRelation::employee_) {
    switch(rec.size()) {
    case 2:
        office = rec[1];
    case 1:
        employee = rec[0];
    }
}
const std::string EmployeeOfficeRelation::table__("Employee_Office_");
const litesql::FieldType EmployeeOfficeRelation::employee_("Employee1","INTEGER",table__);
const litesql::FieldType EmployeeOfficeRelation::office_("Office2","INTEGER",table__);
void EmployeeOfficeRelation::link(const litesql::Database& db, const Employee& o0, const Office& o1) {
    Record values;
    Split fields;
    fields.push_back(employee_.name());
    values.push_back(o0.id);
    fields.push_back(office_.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void EmployeeOfficeRelation::unlink(const litesql::Database& db, const Employee& o0, const Office& o1) {
    db.delete_(table__, (employee_ == o0.id && office_ == o1.id));
}
void EmployeeOfficeRelation::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<EmployeeOfficeRelation::Row> EmployeeOfficeRelation::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(employee_.fullName());
    sel.result(office_.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<EmployeeOfficeRelation::Row>(db, sel);
}
template <> litesql::DataSource<Employee> EmployeeOfficeRelation::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(employee_.fullName());
    sel.where(srcExpr);
    return DataSource<Employee>(db, Employee::id_.in(sel) && expr);
}
template <> litesql::DataSource<Office> EmployeeOfficeRelation::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(office_.fullName());
    sel.where(srcExpr);
    return DataSource<Office>(db, Office::id_.in(sel) && expr);
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
Person::RolesHandle::RolesHandle(const Person& owner)
         : litesql::RelationHandle<Person>(owner) {
}
void Person::RolesHandle::link(const Role& o0) {
    PersonRoleRelationRoles::link(owner->getDatabase(), *owner, o0);
}
void Person::RolesHandle::unlink(const Role& o0) {
    PersonRoleRelationRoles::unlink(owner->getDatabase(), *owner, o0);
}
void Person::RolesHandle::del(const litesql::Expr& expr) {
    PersonRoleRelationRoles::del(owner->getDatabase(), expr);
}
litesql::DataSource<Role> Person::RolesHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return PersonRoleRelationRoles::get<Role>(owner->getDatabase(), expr, (PersonRoleRelationRoles::person_ == owner->id) && srcExpr);
}
litesql::DataSource<PersonRoleRelationRoles::Row> Person::RolesHandle::getRows(const litesql::Expr& expr) {
    return PersonRoleRelationRoles::getRows(owner->getDatabase(), expr && (PersonRoleRelationRoles::person_ == owner->id));
}
const std::string Person::type__("Person");
const std::string Person::table__("Person_");
const std::string Person::sequence__("Person_seq");
const litesql::FieldType Person::id_("id_","INTEGER",table__);
const litesql::FieldType Person::type_("type_","TEXT",table__);
const litesql::FieldType Person::name_("name_","TEXT",table__);
const litesql::FieldType Person::age_("age_","INTEGER",table__);
std::vector < std::pair< std::string, std::string > > Person::sex_values;
const litesql::FieldType Person::sex_("sex_","INTEGER",table__,sex_values);
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
     : litesql::Persistent(db), id(id_), type(type_), name(name_), age(age_), sex(sex_) {
    defaults();
}
Person::Person(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(id_), type(type_), name(name_), age(age_), sex(sex_) {
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
Person::RolesHandle Person::roles() {
    return Person::RolesHandle(*this);
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
    PersonPersonRelationMother::del(*db, (PersonPersonRelationMother::person1_ == id) || (PersonPersonRelationMother::person2_ == id));
    PersonPersonRelationFather::del(*db, (PersonPersonRelationFather::person1_ == id) || (PersonPersonRelationFather::person2_ == id));
    PersonPersonRelationSiblings::del(*db, (PersonPersonRelationSiblings::person1_ == id) || (PersonPersonRelationSiblings::person2_ == id));
    PersonPersonRelationChildren::del(*db, (PersonPersonRelationChildren::person1_ == id) || (PersonPersonRelationChildren::person2_ == id));
    PersonRoleRelationRoles::del(*db, (PersonRoleRelationRoles::person_ == id));
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
const std::string Role::type__("Role");
const std::string Role::table__("Role_");
const std::string Role::sequence__("Role_seq");
const litesql::FieldType Role::id_("id_","INTEGER",table__);
const litesql::FieldType Role::type_("type_","TEXT",table__);
void Role::defaults() {
    id = 0;
}
Role::Role(const litesql::Database& db)
     : litesql::Persistent(db), id(id_), type(type_) {
    defaults();
}
Role::Role(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(id_), type(type_) {
    defaults();
    size_t size = (rec.size() > 2) ? 2 : rec.size();
    switch(size) {
    case 2: type = rec[1];
    case 1: id = rec[0];
    }
}
Role::Role(const Role& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type) {
}
const Role& Role::operator=(const Role& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
std::string Role::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
    tables.push_back(table__);
    litesql::Record fields;
    litesql::Record values;
    fields.push_back(id.name());
    values.push_back(id);
    id.setModified(false);
    fields.push_back(type.name());
    values.push_back(type);
    type.setModified(false);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void Role::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void Role::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
}
void Role::addIDUpdates(Updates& updates) {
}
void Role::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(id_);
    ftypes.push_back(type_);
}
void Role::delRecord() {
    deleteFromTable(table__, id);
}
void Role::delRelations() {
    PersonRoleRelationRoles::del(*db, (PersonRoleRelationRoles::role_ == id));
}
void Role::update() {
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
void Role::del() {
    if (typeIsCorrect() == false) {
        std::auto_ptr<Role> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool Role::typeIsCorrect() {
    return type == type__;
}
std::auto_ptr<Role> Role::upcast() {
    if (type == Student::type__)
        return auto_ptr<Role>(new Student(select<Student>(*db, id_ == id).one()));
    if (type == Employee::type__)
        return auto_ptr<Role>(new Employee(select<Employee>(*db, id_ == id).one()));
    return auto_ptr<Role>(new Role(*this));
}
std::auto_ptr<Role> Role::upcastCopy() {
    Role* np = NULL;
    if (type == "Student")
        np = new Student(*db);
    if (type == "Employee")
        np = new Employee(*db);
    np->id = id;
    np->type = type;
    np->inDatabase = inDatabase;
    if (!np)
        np = new Role(*this);
    return auto_ptr<Role>(np);
}
std::ostream & operator<<(std::ostream& os, Role o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
Student::SchoolHandle::SchoolHandle(const Student& owner)
         : litesql::RelationHandle<Student>(owner) {
}
void Student::SchoolHandle::link(const School& o0) {
    SchoolStudentRelation::link(owner->getDatabase(), o0, *owner);
}
void Student::SchoolHandle::unlink(const School& o0) {
    SchoolStudentRelation::unlink(owner->getDatabase(), o0, *owner);
}
void Student::SchoolHandle::del(const litesql::Expr& expr) {
    SchoolStudentRelation::del(owner->getDatabase(), expr);
}
litesql::DataSource<School> Student::SchoolHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return SchoolStudentRelation::get<School>(owner->getDatabase(), expr, (SchoolStudentRelation::student_ == owner->id) && srcExpr);
}
litesql::DataSource<SchoolStudentRelation::Row> Student::SchoolHandle::getRows(const litesql::Expr& expr) {
    return SchoolStudentRelation::getRows(owner->getDatabase(), expr && (SchoolStudentRelation::student_ == owner->id));
}
const std::string Student::type__("Student");
const std::string Student::table__("Student_");
Student::Student(const litesql::Database& db)
     : Role(db) {
}
Student::Student(const litesql::Database& db, const litesql::Record& rec)
     : Role(db, rec) {
}
Student::Student(const Student& obj)
     : Role(obj) {
}
const Student& Student::operator=(const Student& obj) {
    Role::operator=(obj);
    return *this;
}
Student::SchoolHandle Student::school() {
    return Student::SchoolHandle(*this);
}
std::string Student::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
    tables.push_back(table__);
    litesql::Record fields;
    litesql::Record values;
    fields.push_back("id_");
    values.push_back(id);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return Role::insert(tables, fieldRecs, valueRecs);
}
void Student::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void Student::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    Role::addUpdates(updates);
}
void Student::addIDUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
}
void Student::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    Role::getFieldTypes(ftypes);
}
void Student::delRecord() {
    deleteFromTable(table__, id);
    Role::delRecord();
}
void Student::delRelations() {
    SchoolStudentRelation::del(*db, (SchoolStudentRelation::student_ == id));
}
void Student::update() {
    if (!inDatabase) {
        create();
        return;
    }
    Updates updates;
    addUpdates(updates);
    if (id != oldKey) {
        if (!typeIsCorrect()) 
            upcastCopy()->addIDUpdates(updates);
        else
            addIDUpdates(updates);
    }
    litesql::Persistent::update(updates);
    oldKey = id;
}
void Student::del() {
    if (typeIsCorrect() == false) {
        std::auto_ptr<Student> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool Student::typeIsCorrect() {
    return type == type__;
}
std::auto_ptr<Student> Student::upcast() {
    return auto_ptr<Student>(new Student(*this));
}
std::auto_ptr<Student> Student::upcastCopy() {
    Student* np = NULL;
    np->inDatabase = inDatabase;
    if (!np)
        np = new Student(*this);
    return auto_ptr<Student>(np);
}
std::ostream & operator<<(std::ostream& os, Student o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
Employee::OfficeHandle::OfficeHandle(const Employee& owner)
         : litesql::RelationHandle<Employee>(owner) {
}
void Employee::OfficeHandle::link(const Office& o0) {
    EmployeeOfficeRelation::link(owner->getDatabase(), *owner, o0);
}
void Employee::OfficeHandle::unlink(const Office& o0) {
    EmployeeOfficeRelation::unlink(owner->getDatabase(), *owner, o0);
}
void Employee::OfficeHandle::del(const litesql::Expr& expr) {
    EmployeeOfficeRelation::del(owner->getDatabase(), expr);
}
litesql::DataSource<Office> Employee::OfficeHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return EmployeeOfficeRelation::get<Office>(owner->getDatabase(), expr, (EmployeeOfficeRelation::employee_ == owner->id) && srcExpr);
}
litesql::DataSource<EmployeeOfficeRelation::Row> Employee::OfficeHandle::getRows(const litesql::Expr& expr) {
    return EmployeeOfficeRelation::getRows(owner->getDatabase(), expr && (EmployeeOfficeRelation::employee_ == owner->id));
}
const std::string Employee::type__("Employee");
const std::string Employee::table__("Employee_");
Employee::Employee(const litesql::Database& db)
     : Role(db) {
}
Employee::Employee(const litesql::Database& db, const litesql::Record& rec)
     : Role(db, rec) {
}
Employee::Employee(const Employee& obj)
     : Role(obj) {
}
const Employee& Employee::operator=(const Employee& obj) {
    Role::operator=(obj);
    return *this;
}
Employee::OfficeHandle Employee::office() {
    return Employee::OfficeHandle(*this);
}
std::string Employee::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
    tables.push_back(table__);
    litesql::Record fields;
    litesql::Record values;
    fields.push_back("id_");
    values.push_back(id);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return Role::insert(tables, fieldRecs, valueRecs);
}
void Employee::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void Employee::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    Role::addUpdates(updates);
}
void Employee::addIDUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
}
void Employee::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    Role::getFieldTypes(ftypes);
}
void Employee::delRecord() {
    deleteFromTable(table__, id);
    Role::delRecord();
}
void Employee::delRelations() {
    EmployeeOfficeRelation::del(*db, (EmployeeOfficeRelation::employee_ == id));
}
void Employee::update() {
    if (!inDatabase) {
        create();
        return;
    }
    Updates updates;
    addUpdates(updates);
    if (id != oldKey) {
        if (!typeIsCorrect()) 
            upcastCopy()->addIDUpdates(updates);
        else
            addIDUpdates(updates);
    }
    litesql::Persistent::update(updates);
    oldKey = id;
}
void Employee::del() {
    if (typeIsCorrect() == false) {
        std::auto_ptr<Employee> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool Employee::typeIsCorrect() {
    return type == type__;
}
std::auto_ptr<Employee> Employee::upcast() {
    return auto_ptr<Employee>(new Employee(*this));
}
std::auto_ptr<Employee> Employee::upcastCopy() {
    Employee* np = NULL;
    np->inDatabase = inDatabase;
    if (!np)
        np = new Employee(*this);
    return auto_ptr<Employee>(np);
}
std::ostream & operator<<(std::ostream& os, Employee o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
School::StudentsHandle::StudentsHandle(const School& owner)
         : litesql::RelationHandle<School>(owner) {
}
void School::StudentsHandle::link(const Student& o0) {
    SchoolStudentRelation::link(owner->getDatabase(), *owner, o0);
}
void School::StudentsHandle::unlink(const Student& o0) {
    SchoolStudentRelation::unlink(owner->getDatabase(), *owner, o0);
}
void School::StudentsHandle::del(const litesql::Expr& expr) {
    SchoolStudentRelation::del(owner->getDatabase(), expr);
}
litesql::DataSource<Student> School::StudentsHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return SchoolStudentRelation::get<Student>(owner->getDatabase(), expr, (SchoolStudentRelation::school_ == owner->id) && srcExpr);
}
litesql::DataSource<SchoolStudentRelation::Row> School::StudentsHandle::getRows(const litesql::Expr& expr) {
    return SchoolStudentRelation::getRows(owner->getDatabase(), expr && (SchoolStudentRelation::school_ == owner->id));
}
const std::string School::type__("School");
const std::string School::table__("School_");
const std::string School::sequence__("School_seq");
const litesql::FieldType School::id_("id_","INTEGER",table__);
const litesql::FieldType School::type_("type_","TEXT",table__);
const litesql::FieldType School::name_("name_","TEXT",table__);
void School::defaults() {
    id = 0;
}
School::School(const litesql::Database& db)
     : litesql::Persistent(db), id(id_), type(type_), name(name_) {
    defaults();
}
School::School(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(id_), type(type_), name(name_) {
    defaults();
    size_t size = (rec.size() > 3) ? 3 : rec.size();
    switch(size) {
    case 3: name = rec[2];
    case 2: type = rec[1];
    case 1: id = rec[0];
    }
}
School::School(const School& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), name(obj.name) {
}
const School& School::operator=(const School& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        name = obj.name;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
School::StudentsHandle School::students() {
    return School::StudentsHandle(*this);
}
std::string School::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
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
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void School::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void School::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
    updateField(updates, table__, name);
}
void School::addIDUpdates(Updates& updates) {
}
void School::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(id_);
    ftypes.push_back(type_);
    ftypes.push_back(name_);
}
void School::delRecord() {
    deleteFromTable(table__, id);
}
void School::delRelations() {
    SchoolStudentRelation::del(*db, (SchoolStudentRelation::school_ == id));
}
void School::update() {
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
void School::del() {
    if (typeIsCorrect() == false) {
        std::auto_ptr<School> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool School::typeIsCorrect() {
    return type == type__;
}
std::auto_ptr<School> School::upcast() {
    return auto_ptr<School>(new School(*this));
}
std::auto_ptr<School> School::upcastCopy() {
    School* np = NULL;
    np->id = id;
    np->type = type;
    np->name = name;
    np->inDatabase = inDatabase;
    if (!np)
        np = new School(*this);
    return auto_ptr<School>(np);
}
std::ostream & operator<<(std::ostream& os, School o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.name.name() << " = " << o.name << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
Office::EmployeesHandle::EmployeesHandle(const Office& owner)
         : litesql::RelationHandle<Office>(owner) {
}
void Office::EmployeesHandle::link(const Employee& o0) {
    EmployeeOfficeRelation::link(owner->getDatabase(), o0, *owner);
}
void Office::EmployeesHandle::unlink(const Employee& o0) {
    EmployeeOfficeRelation::unlink(owner->getDatabase(), o0, *owner);
}
void Office::EmployeesHandle::del(const litesql::Expr& expr) {
    EmployeeOfficeRelation::del(owner->getDatabase(), expr);
}
litesql::DataSource<Employee> Office::EmployeesHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return EmployeeOfficeRelation::get<Employee>(owner->getDatabase(), expr, (EmployeeOfficeRelation::office_ == owner->id) && srcExpr);
}
litesql::DataSource<EmployeeOfficeRelation::Row> Office::EmployeesHandle::getRows(const litesql::Expr& expr) {
    return EmployeeOfficeRelation::getRows(owner->getDatabase(), expr && (EmployeeOfficeRelation::office_ == owner->id));
}
const std::string Office::type__("Office");
const std::string Office::table__("Office_");
const std::string Office::sequence__("Office_seq");
const litesql::FieldType Office::id_("id_","INTEGER",table__);
const litesql::FieldType Office::type_("type_","TEXT",table__);
void Office::defaults() {
    id = 0;
}
Office::Office(const litesql::Database& db)
     : litesql::Persistent(db), id(id_), type(type_) {
    defaults();
}
Office::Office(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(id_), type(type_) {
    defaults();
    size_t size = (rec.size() > 2) ? 2 : rec.size();
    switch(size) {
    case 2: type = rec[1];
    case 1: id = rec[0];
    }
}
Office::Office(const Office& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type) {
}
const Office& Office::operator=(const Office& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
Office::EmployeesHandle Office::employees() {
    return Office::EmployeesHandle(*this);
}
std::string Office::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
    tables.push_back(table__);
    litesql::Record fields;
    litesql::Record values;
    fields.push_back(id.name());
    values.push_back(id);
    id.setModified(false);
    fields.push_back(type.name());
    values.push_back(type);
    type.setModified(false);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void Office::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void Office::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
}
void Office::addIDUpdates(Updates& updates) {
}
void Office::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(id_);
    ftypes.push_back(type_);
}
void Office::delRecord() {
    deleteFromTable(table__, id);
}
void Office::delRelations() {
    EmployeeOfficeRelation::del(*db, (EmployeeOfficeRelation::office_ == id));
}
void Office::update() {
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
void Office::del() {
    if (typeIsCorrect() == false) {
        std::auto_ptr<Office> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool Office::typeIsCorrect() {
    return type == type__;
}
std::auto_ptr<Office> Office::upcast() {
    return auto_ptr<Office>(new Office(*this));
}
std::auto_ptr<Office> Office::upcastCopy() {
    Office* np = NULL;
    np->id = id;
    np->type = type;
    np->inDatabase = inDatabase;
    if (!np)
        np = new Office(*this);
    return auto_ptr<Office>(np);
}
std::ostream & operator<<(std::ostream& os, Office o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
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
    res.push_back(Database::SchemaItem(Person::table__,"table","CREATE TABLE " + Person::table__ + " (" + Person::id_.name() + " " + backend->getRowIDType() + "," + Person::type_.name() + " " + Person::type_.type() + "," + Person::name_.name() + " " + Person::name_.type() + "," + Person::age_.name() + " " + Person::age_.type() + "," + Person::sex_.name() + " " + Person::sex_.type() + ")"));
    if (backend->supportsSequences())
    res.push_back(Database::SchemaItem(Person::sequence__,"sequence","CREATE SEQUENCE " + Person::sequence__ + " START 1 INCREMENT 1"));
    res.push_back(Database::SchemaItem(Role::table__,"table","CREATE TABLE " + Role::table__ + " (" + Role::id_.name() + " " + backend->getRowIDType() + "," + Role::type_.name() + " " + Role::type_.type() + ")"));
    if (backend->supportsSequences())
    res.push_back(Database::SchemaItem(Role::sequence__,"sequence","CREATE SEQUENCE " + Role::sequence__ + " START 1 INCREMENT 1"));
    res.push_back(Database::SchemaItem(Student::table__,"table","CREATE TABLE " + Student::table__ + " (" + Student::id_.name() + " " + backend->getRowIDType() + ")"));
    res.push_back(Database::SchemaItem(Employee::table__,"table","CREATE TABLE " + Employee::table__ + " (" + Employee::id_.name() + " " + backend->getRowIDType() + ")"));
    res.push_back(Database::SchemaItem(School::table__,"table","CREATE TABLE " + School::table__ + " (" + School::id_.name() + " " + backend->getRowIDType() + "," + School::type_.name() + " " + School::type_.type() + "," + School::name_.name() + " " + School::name_.type() + ")"));
    if (backend->supportsSequences())
    res.push_back(Database::SchemaItem(School::sequence__,"sequence","CREATE SEQUENCE " + School::sequence__ + " START 1 INCREMENT 1"));
    res.push_back(Database::SchemaItem(Office::table__,"table","CREATE TABLE " + Office::table__ + " (" + Office::id_.name() + " " + backend->getRowIDType() + "," + Office::type_.name() + " " + Office::type_.type() + ")"));
    if (backend->supportsSequences())
    res.push_back(Database::SchemaItem(Office::sequence__,"sequence","CREATE SEQUENCE " + Office::sequence__ + " START 1 INCREMENT 1"));
    res.push_back(Database::SchemaItem(PersonPersonRelationMother::table__,"table","CREATE TABLE " + PersonPersonRelationMother::table__ + " (" + PersonPersonRelationMother::person1_.name() + " " + PersonPersonRelationMother::person1_.type() + " UNIQUE"+ "," + PersonPersonRelationMother::person2_.name() + " " + PersonPersonRelationMother::person2_.type() + ")"));
    res.push_back(Database::SchemaItem("Person_Person_Mother_all_idx","index","CREATE INDEX Person_Person_Mother_all_idx ON " + PersonPersonRelationMother::table__ + " (" + PersonPersonRelationMother::person1_.name() + "," + PersonPersonRelationMother::person2_.name() + ")"));
    res.push_back(Database::SchemaItem("_0017af524322b4fb41b524f714af13a2","index","CREATE INDEX _0017af524322b4fb41b524f714af13a2 ON " + PersonPersonRelationMother::table__ + " (" + PersonPersonRelationMother::person1_.name() + ")"));
    res.push_back(Database::SchemaItem("_2fab06a484b54483a4525f247fa1b965","index","CREATE INDEX _2fab06a484b54483a4525f247fa1b965 ON " + PersonPersonRelationMother::table__ + " (" + PersonPersonRelationMother::person2_.name() + ")"));
    res.push_back(Database::SchemaItem(PersonPersonRelationFather::table__,"table","CREATE TABLE " + PersonPersonRelationFather::table__ + " (" + PersonPersonRelationFather::person1_.name() + " " + PersonPersonRelationFather::person1_.type() + " UNIQUE"+ "," + PersonPersonRelationFather::person2_.name() + " " + PersonPersonRelationFather::person2_.type() + ")"));
    res.push_back(Database::SchemaItem("Person_Person_Father_all_idx","index","CREATE INDEX Person_Person_Father_all_idx ON " + PersonPersonRelationFather::table__ + " (" + PersonPersonRelationFather::person1_.name() + "," + PersonPersonRelationFather::person2_.name() + ")"));
    res.push_back(Database::SchemaItem("_aa758c892838ec42580562dca1f4a19c","index","CREATE INDEX _aa758c892838ec42580562dca1f4a19c ON " + PersonPersonRelationFather::table__ + " (" + PersonPersonRelationFather::person1_.name() + ")"));
    res.push_back(Database::SchemaItem("_42c5a7f3557b70ad11c05d944df009ab","index","CREATE INDEX _42c5a7f3557b70ad11c05d944df009ab ON " + PersonPersonRelationFather::table__ + " (" + PersonPersonRelationFather::person2_.name() + ")"));
    res.push_back(Database::SchemaItem(PersonPersonRelationSiblings::table__,"table","CREATE TABLE " + PersonPersonRelationSiblings::table__ + " (" + PersonPersonRelationSiblings::person1_.name() + " " + PersonPersonRelationSiblings::person1_.type()+ "," + PersonPersonRelationSiblings::person2_.name() + " " + PersonPersonRelationSiblings::person2_.type() + ")"));
    res.push_back(Database::SchemaItem("Person_Person_Siblings_all_idx","index","CREATE INDEX Person_Person_Siblings_all_idx ON " + PersonPersonRelationSiblings::table__ + " (" + PersonPersonRelationSiblings::person1_.name() + "," + PersonPersonRelationSiblings::person2_.name() + ")"));
    res.push_back(Database::SchemaItem("_e0777b3d538001a3f80547a023adf52b","index","CREATE INDEX _e0777b3d538001a3f80547a023adf52b ON " + PersonPersonRelationSiblings::table__ + " (" + PersonPersonRelationSiblings::person1_.name() + ")"));
    res.push_back(Database::SchemaItem("_c378e760458b893ba93eccc716f4e73f","index","CREATE INDEX _c378e760458b893ba93eccc716f4e73f ON " + PersonPersonRelationSiblings::table__ + " (" + PersonPersonRelationSiblings::person2_.name() + ")"));
    res.push_back(Database::SchemaItem(PersonPersonRelationChildren::table__,"table","CREATE TABLE " + PersonPersonRelationChildren::table__ + " (" + PersonPersonRelationChildren::person1_.name() + " " + PersonPersonRelationChildren::person1_.type()+ "," + PersonPersonRelationChildren::person2_.name() + " " + PersonPersonRelationChildren::person2_.type() + ")"));
    res.push_back(Database::SchemaItem("Person_Person_Children_all_idx","index","CREATE INDEX Person_Person_Children_all_idx ON " + PersonPersonRelationChildren::table__ + " (" + PersonPersonRelationChildren::person1_.name() + "," + PersonPersonRelationChildren::person2_.name() + ")"));
    res.push_back(Database::SchemaItem("_b36d653f0e416ef6c882553b4b2b4f48","index","CREATE INDEX _b36d653f0e416ef6c882553b4b2b4f48 ON " + PersonPersonRelationChildren::table__ + " (" + PersonPersonRelationChildren::person1_.name() + ")"));
    res.push_back(Database::SchemaItem("_6c7267723f700ce335ac5843d9dd9715","index","CREATE INDEX _6c7267723f700ce335ac5843d9dd9715 ON " + PersonPersonRelationChildren::table__ + " (" + PersonPersonRelationChildren::person2_.name() + ")"));
    res.push_back(Database::SchemaItem(PersonRoleRelationRoles::table__,"table","CREATE TABLE " + PersonRoleRelationRoles::table__ + " (" + PersonRoleRelationRoles::person_.name() + " " + PersonRoleRelationRoles::person_.type()+ "," + PersonRoleRelationRoles::role_.name() + " " + PersonRoleRelationRoles::role_.type() + ")"));
    res.push_back(Database::SchemaItem("Person_Role_Roles_all_idx","index","CREATE INDEX Person_Role_Roles_all_idx ON " + PersonRoleRelationRoles::table__ + " (" + PersonRoleRelationRoles::person_.name() + "," + PersonRoleRelationRoles::role_.name() + ")"));
    res.push_back(Database::SchemaItem("Person_Role_Roles_person__idx","index","CREATE INDEX Person_Role_Roles_person__idx ON " + PersonRoleRelationRoles::table__ + " (" + PersonRoleRelationRoles::person_.name() + ")"));
    res.push_back(Database::SchemaItem("Person_Role_Roles_role__idx","index","CREATE INDEX Person_Role_Roles_role__idx ON " + PersonRoleRelationRoles::table__ + " (" + PersonRoleRelationRoles::role_.name() + ")"));
    res.push_back(Database::SchemaItem(SchoolStudentRelation::table__,"table","CREATE TABLE " + SchoolStudentRelation::table__ + " (" + SchoolStudentRelation::school_.name() + " " + SchoolStudentRelation::school_.type()+ "," + SchoolStudentRelation::student_.name() + " " + SchoolStudentRelation::student_.type() + " UNIQUE" + ")"));
    res.push_back(Database::SchemaItem("School_Student__all_idx","index","CREATE INDEX School_Student__all_idx ON " + SchoolStudentRelation::table__ + " (" + SchoolStudentRelation::school_.name() + "," + SchoolStudentRelation::student_.name() + ")"));
    res.push_back(Database::SchemaItem("School_Student__school__idx","index","CREATE INDEX School_Student__school__idx ON " + SchoolStudentRelation::table__ + " (" + SchoolStudentRelation::school_.name() + ")"));
    res.push_back(Database::SchemaItem("School_Student__student__idx","index","CREATE INDEX School_Student__student__idx ON " + SchoolStudentRelation::table__ + " (" + SchoolStudentRelation::student_.name() + ")"));
    res.push_back(Database::SchemaItem(EmployeeOfficeRelation::table__,"table","CREATE TABLE " + EmployeeOfficeRelation::table__ + " (" + EmployeeOfficeRelation::employee_.name() + " " + EmployeeOfficeRelation::employee_.type()+ "," + EmployeeOfficeRelation::office_.name() + " " + EmployeeOfficeRelation::office_.type() + ")"));
    res.push_back(Database::SchemaItem("Employee_Office__all_idx","index","CREATE INDEX Employee_Office__all_idx ON " + EmployeeOfficeRelation::table__ + " (" + EmployeeOfficeRelation::employee_.name() + "," + EmployeeOfficeRelation::office_.name() + ")"));
    res.push_back(Database::SchemaItem("Employee_Office__employee__idx","index","CREATE INDEX Employee_Office__employee__idx ON " + EmployeeOfficeRelation::table__ + " (" + EmployeeOfficeRelation::employee_.name() + ")"));
    res.push_back(Database::SchemaItem("Employee_Office__office__idx","index","CREATE INDEX Employee_Office__office__idx ON " + EmployeeOfficeRelation::table__ + " (" + EmployeeOfficeRelation::office_.name() + ")"));
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

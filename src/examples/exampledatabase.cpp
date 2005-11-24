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
const std::string PersonPersonRelationMother::table__("Person_Person_Mother");
const litesql::FieldType PersonPersonRelationMother::person1("Person1","INTEGER",table__);
const litesql::FieldType PersonPersonRelationMother::person2("Person2","INTEGER",table__);
void PersonPersonRelationMother::link(const litesql::Database& db, const Person& o0, const Person& o1) {
    Record values;
    Split fields;
    fields.push_back(person1.name());
    values.push_back(o0.id);
    fields.push_back(person2.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void PersonPersonRelationMother::unlink(const litesql::Database& db, const Person& o0, const Person& o1) {
    db.delete_(table__, (person1 == o0.id && person2 == o1.id));
}
void PersonPersonRelationMother::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<PersonPersonRelationMother::Row> PersonPersonRelationMother::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(person1.fullName());
    sel.result(person2.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<PersonPersonRelationMother::Row>(db, sel);
}
litesql::DataSource<Person> PersonPersonRelationMother::getPerson1(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(person1.fullName());
    sel.where(srcExpr);
    return DataSource<Person>(db, Person::Id.in(sel) && expr);
}
litesql::DataSource<Person> PersonPersonRelationMother::getPerson2(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(person2.fullName());
    sel.where(srcExpr);
    return DataSource<Person>(db, Person::Id.in(sel) && expr);
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
const std::string PersonPersonRelationFather::table__("Person_Person_Father");
const litesql::FieldType PersonPersonRelationFather::person1("Person1","INTEGER",table__);
const litesql::FieldType PersonPersonRelationFather::person2("Person2","INTEGER",table__);
void PersonPersonRelationFather::link(const litesql::Database& db, const Person& o0, const Person& o1) {
    Record values;
    Split fields;
    fields.push_back(person1.name());
    values.push_back(o0.id);
    fields.push_back(person2.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void PersonPersonRelationFather::unlink(const litesql::Database& db, const Person& o0, const Person& o1) {
    db.delete_(table__, (person1 == o0.id && person2 == o1.id));
}
void PersonPersonRelationFather::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<PersonPersonRelationFather::Row> PersonPersonRelationFather::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(person1.fullName());
    sel.result(person2.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<PersonPersonRelationFather::Row>(db, sel);
}
litesql::DataSource<Person> PersonPersonRelationFather::getPerson1(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(person1.fullName());
    sel.where(srcExpr);
    return DataSource<Person>(db, Person::Id.in(sel) && expr);
}
litesql::DataSource<Person> PersonPersonRelationFather::getPerson2(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(person2.fullName());
    sel.where(srcExpr);
    return DataSource<Person>(db, Person::Id.in(sel) && expr);
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
const std::string PersonPersonRelationSiblings::table__("Person_Person_Siblings");
const litesql::FieldType PersonPersonRelationSiblings::person1("Person1","INTEGER",table__);
const litesql::FieldType PersonPersonRelationSiblings::person2("Person2","INTEGER",table__);
void PersonPersonRelationSiblings::link(const litesql::Database& db, const Person& o0, const Person& o1) {
    Record values;
    Split fields;
    fields.push_back(person1.name());
    values.push_back(o0.id);
    fields.push_back(person2.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
    fields.clear();
    values.clear();
    fields.push_back(person1.name());
    values.push_back(o1.id);
    fields.push_back(person2.name());
    values.push_back(o0.id);
    db.insert(table__, values, fields);
}
void PersonPersonRelationSiblings::unlink(const litesql::Database& db, const Person& o0, const Person& o1) {
    db.delete_(table__, (person1 == o0.id && person2 == o1.id));
    db.delete_(table__, (person1 == o1.id && person2 == o0.id));
}
void PersonPersonRelationSiblings::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<PersonPersonRelationSiblings::Row> PersonPersonRelationSiblings::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(person1.fullName());
    sel.result(person2.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<PersonPersonRelationSiblings::Row>(db, sel);
}
litesql::DataSource<Person> PersonPersonRelationSiblings::getPerson1(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(person1.fullName());
    sel.where(srcExpr);
    return DataSource<Person>(db, Person::Id.in(sel) && expr);
}
litesql::DataSource<Person> PersonPersonRelationSiblings::getPerson2(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(person2.fullName());
    sel.where(srcExpr);
    return DataSource<Person>(db, Person::Id.in(sel) && expr);
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
const std::string PersonPersonRelationChildren::table__("Person_Person_Children");
const litesql::FieldType PersonPersonRelationChildren::person1("Person1","INTEGER",table__);
const litesql::FieldType PersonPersonRelationChildren::person2("Person2","INTEGER",table__);
void PersonPersonRelationChildren::link(const litesql::Database& db, const Person& o0, const Person& o1) {
    Record values;
    Split fields;
    fields.push_back(person1.name());
    values.push_back(o0.id);
    fields.push_back(person2.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void PersonPersonRelationChildren::unlink(const litesql::Database& db, const Person& o0, const Person& o1) {
    db.delete_(table__, (person1 == o0.id && person2 == o1.id));
}
void PersonPersonRelationChildren::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<PersonPersonRelationChildren::Row> PersonPersonRelationChildren::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(person1.fullName());
    sel.result(person2.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<PersonPersonRelationChildren::Row>(db, sel);
}
litesql::DataSource<Person> PersonPersonRelationChildren::getPerson1(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(person1.fullName());
    sel.where(srcExpr);
    return DataSource<Person>(db, Person::Id.in(sel) && expr);
}
litesql::DataSource<Person> PersonPersonRelationChildren::getPerson2(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(person2.fullName());
    sel.where(srcExpr);
    return DataSource<Person>(db, Person::Id.in(sel) && expr);
}
RoleRelation::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : role(RoleRelation::role), person(RoleRelation::person) {
    switch(rec.size()) {
    case 2:
        role = rec[1];
    case 1:
        person = rec[0];
    }
}
const std::string RoleRelation::table__("Person_Role_Roles");
const litesql::FieldType RoleRelation::person("Person1","INTEGER",table__);
const litesql::FieldType RoleRelation::role("Role2","INTEGER",table__);
void RoleRelation::link(const litesql::Database& db, const Person& o0, const Role& o1) {
    Record values;
    Split fields;
    fields.push_back(person.name());
    values.push_back(o0.id);
    fields.push_back(role.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void RoleRelation::unlink(const litesql::Database& db, const Person& o0, const Role& o1) {
    db.delete_(table__, (person == o0.id && role == o1.id));
}
void RoleRelation::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<RoleRelation::Row> RoleRelation::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(person.fullName());
    sel.result(role.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<RoleRelation::Row>(db, sel);
}
template <> litesql::DataSource<Person> RoleRelation::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(person.fullName());
    sel.where(srcExpr);
    return DataSource<Person>(db, Person::Id.in(sel) && expr);
}
template <> litesql::DataSource<Role> RoleRelation::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(role.fullName());
    sel.where(srcExpr);
    return DataSource<Role>(db, Role::Id.in(sel) && expr);
}
SchoolStudentRelation::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : student(SchoolStudentRelation::student), school(SchoolStudentRelation::school) {
    switch(rec.size()) {
    case 2:
        student = rec[1];
    case 1:
        school = rec[0];
    }
}
const std::string SchoolStudentRelation::table__("School_Student_");
const litesql::FieldType SchoolStudentRelation::school("School1","INTEGER",table__);
const litesql::FieldType SchoolStudentRelation::student("Student2","INTEGER",table__);
void SchoolStudentRelation::link(const litesql::Database& db, const School& o0, const Student& o1) {
    Record values;
    Split fields;
    fields.push_back(school.name());
    values.push_back(o0.id);
    fields.push_back(student.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void SchoolStudentRelation::unlink(const litesql::Database& db, const School& o0, const Student& o1) {
    db.delete_(table__, (school == o0.id && student == o1.id));
}
void SchoolStudentRelation::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<SchoolStudentRelation::Row> SchoolStudentRelation::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(school.fullName());
    sel.result(student.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<SchoolStudentRelation::Row>(db, sel);
}
template <> litesql::DataSource<School> SchoolStudentRelation::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(school.fullName());
    sel.where(srcExpr);
    return DataSource<School>(db, School::Id.in(sel) && expr);
}
template <> litesql::DataSource<Student> SchoolStudentRelation::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(student.fullName());
    sel.where(srcExpr);
    return DataSource<Student>(db, Student::Id.in(sel) && expr);
}
EmployeeOfficeRelation::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : office(EmployeeOfficeRelation::office), employee(EmployeeOfficeRelation::employee) {
    switch(rec.size()) {
    case 2:
        office = rec[1];
    case 1:
        employee = rec[0];
    }
}
const std::string EmployeeOfficeRelation::table__("Employee_Office_");
const litesql::FieldType EmployeeOfficeRelation::employee("Employee1","INTEGER",table__);
const litesql::FieldType EmployeeOfficeRelation::office("Office2","INTEGER",table__);
void EmployeeOfficeRelation::link(const litesql::Database& db, const Employee& o0, const Office& o1) {
    Record values;
    Split fields;
    fields.push_back(employee.name());
    values.push_back(o0.id);
    fields.push_back(office.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void EmployeeOfficeRelation::unlink(const litesql::Database& db, const Employee& o0, const Office& o1) {
    db.delete_(table__, (employee == o0.id && office == o1.id));
}
void EmployeeOfficeRelation::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<EmployeeOfficeRelation::Row> EmployeeOfficeRelation::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(employee.fullName());
    sel.result(office.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<EmployeeOfficeRelation::Row>(db, sel);
}
template <> litesql::DataSource<Employee> EmployeeOfficeRelation::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(employee.fullName());
    sel.where(srcExpr);
    return DataSource<Employee>(db, Employee::Id.in(sel) && expr);
}
template <> litesql::DataSource<Office> EmployeeOfficeRelation::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(office.fullName());
    sel.where(srcExpr);
    return DataSource<Office>(db, Office::Id.in(sel) && expr);
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
    return PersonPersonRelationMother::getPerson2(owner->getDatabase(), expr, (PersonPersonRelationMother::person1 == owner->id) && srcExpr);
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
litesql::DataSource<Person> Person::FatherHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return PersonPersonRelationFather::getPerson2(owner->getDatabase(), expr, (PersonPersonRelationFather::person1 == owner->id) && srcExpr);
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
litesql::DataSource<Person> Person::SiblingsHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return PersonPersonRelationSiblings::getPerson2(owner->getDatabase(), expr, (PersonPersonRelationSiblings::person1 == owner->id) && srcExpr);
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
litesql::DataSource<Person> Person::ChildrenHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return PersonPersonRelationChildren::getPerson2(owner->getDatabase(), expr, (PersonPersonRelationChildren::person1 == owner->id) && srcExpr);
}
litesql::DataSource<PersonPersonRelationChildren::Row> Person::ChildrenHandle::getRows(const litesql::Expr& expr) {
    return PersonPersonRelationChildren::getRows(owner->getDatabase(), expr && (PersonPersonRelationChildren::person1 == owner->id));
}
Person::RolesHandle::RolesHandle(const Person& owner)
         : litesql::RelationHandle<Person>(owner) {
}
void Person::RolesHandle::link(const Role& o0) {
    RoleRelation::link(owner->getDatabase(), *owner, o0);
}
void Person::RolesHandle::unlink(const Role& o0) {
    RoleRelation::unlink(owner->getDatabase(), *owner, o0);
}
void Person::RolesHandle::del(const litesql::Expr& expr) {
    RoleRelation::del(owner->getDatabase(), expr);
}
litesql::DataSource<Role> Person::RolesHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return RoleRelation::get<Role>(owner->getDatabase(), expr, (RoleRelation::person == owner->id) && srcExpr);
}
litesql::DataSource<RoleRelation::Row> Person::RolesHandle::getRows(const litesql::Expr& expr) {
    return RoleRelation::getRows(owner->getDatabase(), expr && (RoleRelation::person == owner->id));
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
    PersonPersonRelationMother::del(*db, (PersonPersonRelationMother::person1 == id) || (PersonPersonRelationMother::person2 == id));
    PersonPersonRelationFather::del(*db, (PersonPersonRelationFather::person1 == id) || (PersonPersonRelationFather::person2 == id));
    PersonPersonRelationSiblings::del(*db, (PersonPersonRelationSiblings::person1 == id) || (PersonPersonRelationSiblings::person2 == id));
    PersonPersonRelationChildren::del(*db, (PersonPersonRelationChildren::person1 == id) || (PersonPersonRelationChildren::person2 == id));
    RoleRelation::del(*db, (RoleRelation::person == id));
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
Role::PersonHandle::PersonHandle(const Role& owner)
         : litesql::RelationHandle<Role>(owner) {
}
void Role::PersonHandle::link(const Person& o0) {
    RoleRelation::link(owner->getDatabase(), o0, *owner);
}
void Role::PersonHandle::unlink(const Person& o0) {
    RoleRelation::unlink(owner->getDatabase(), o0, *owner);
}
void Role::PersonHandle::del(const litesql::Expr& expr) {
    RoleRelation::del(owner->getDatabase(), expr);
}
litesql::DataSource<Person> Role::PersonHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return RoleRelation::get<Person>(owner->getDatabase(), expr, (RoleRelation::role == owner->id) && srcExpr);
}
litesql::DataSource<RoleRelation::Row> Role::PersonHandle::getRows(const litesql::Expr& expr) {
    return RoleRelation::getRows(owner->getDatabase(), expr && (RoleRelation::role == owner->id));
}
const std::string Role::type__("Role");
const std::string Role::table__("Role_");
const std::string Role::sequence__("Role_seq");
const litesql::FieldType Role::Id("id_","INTEGER",table__);
const litesql::FieldType Role::Type("type_","TEXT",table__);
void Role::defaults() {
    id = 0;
}
Role::Role(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type) {
    defaults();
}
Role::Role(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type) {
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
Role::PersonHandle Role::person() {
    return Role::PersonHandle(*this);
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
    ftypes.push_back(Id);
    ftypes.push_back(Type);
}
void Role::delRecord() {
    deleteFromTable(table__, id);
}
void Role::delRelations() {
    RoleRelation::del(*db, (RoleRelation::role == id));
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
        return auto_ptr<Role>(new Student(select<Student>(*db, Id == id).one()));
    if (type == Employee::type__)
        return auto_ptr<Role>(new Employee(select<Employee>(*db, Id == id).one()));
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
    return SchoolStudentRelation::get<School>(owner->getDatabase(), expr, (SchoolStudentRelation::student == owner->id) && srcExpr);
}
litesql::DataSource<SchoolStudentRelation::Row> Student::SchoolHandle::getRows(const litesql::Expr& expr) {
    return SchoolStudentRelation::getRows(owner->getDatabase(), expr && (SchoolStudentRelation::student == owner->id));
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
    SchoolStudentRelation::del(*db, (SchoolStudentRelation::student == id));
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
    return EmployeeOfficeRelation::get<Office>(owner->getDatabase(), expr, (EmployeeOfficeRelation::employee == owner->id) && srcExpr);
}
litesql::DataSource<EmployeeOfficeRelation::Row> Employee::OfficeHandle::getRows(const litesql::Expr& expr) {
    return EmployeeOfficeRelation::getRows(owner->getDatabase(), expr && (EmployeeOfficeRelation::employee == owner->id));
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
    EmployeeOfficeRelation::del(*db, (EmployeeOfficeRelation::employee == id));
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
    return SchoolStudentRelation::get<Student>(owner->getDatabase(), expr, (SchoolStudentRelation::school == owner->id) && srcExpr);
}
litesql::DataSource<SchoolStudentRelation::Row> School::StudentsHandle::getRows(const litesql::Expr& expr) {
    return SchoolStudentRelation::getRows(owner->getDatabase(), expr && (SchoolStudentRelation::school == owner->id));
}
const std::string School::type__("School");
const std::string School::table__("School_");
const std::string School::sequence__("School_seq");
const litesql::FieldType School::Id("id_","INTEGER",table__);
const litesql::FieldType School::Type("type_","TEXT",table__);
const litesql::FieldType School::Name("name_","TEXT",table__);
void School::defaults() {
    id = 0;
}
School::School(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), name(Name) {
    defaults();
}
School::School(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), name(Name) {
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
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(Name);
}
void School::delRecord() {
    deleteFromTable(table__, id);
}
void School::delRelations() {
    SchoolStudentRelation::del(*db, (SchoolStudentRelation::school == id));
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
    return EmployeeOfficeRelation::get<Employee>(owner->getDatabase(), expr, (EmployeeOfficeRelation::office == owner->id) && srcExpr);
}
litesql::DataSource<EmployeeOfficeRelation::Row> Office::EmployeesHandle::getRows(const litesql::Expr& expr) {
    return EmployeeOfficeRelation::getRows(owner->getDatabase(), expr && (EmployeeOfficeRelation::office == owner->id));
}
const std::string Office::type__("Office");
const std::string Office::table__("Office_");
const std::string Office::sequence__("Office_seq");
const litesql::FieldType Office::Id("id_","INTEGER",table__);
const litesql::FieldType Office::Type("type_","TEXT",table__);
void Office::defaults() {
    id = 0;
}
Office::Office(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type) {
    defaults();
}
Office::Office(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type) {
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
    ftypes.push_back(Id);
    ftypes.push_back(Type);
}
void Office::delRecord() {
    deleteFromTable(table__, id);
}
void Office::delRelations() {
    EmployeeOfficeRelation::del(*db, (EmployeeOfficeRelation::office == id));
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
    res.push_back(Database::SchemaItem(Person::table__,"table","CREATE TABLE " + Person::table__ + " (" + Person::Id.name() + " " + backend->getRowIDType() + "," + Person::Type.name() + " " + Person::Type.type() + "," + Person::Name.name() + " " + Person::Name.type() + "," + Person::Age.name() + " " + Person::Age.type() + "," + Person::Sex.name() + " " + Person::Sex.type() + ")"));
    if (backend->supportsSequences())
    res.push_back(Database::SchemaItem(Person::sequence__,"sequence","CREATE SEQUENCE " + Person::sequence__ + " START 1 INCREMENT 1"));
    res.push_back(Database::SchemaItem(Role::table__,"table","CREATE TABLE " + Role::table__ + " (" + Role::Id.name() + " " + backend->getRowIDType() + "," + Role::Type.name() + " " + Role::Type.type() + ")"));
    if (backend->supportsSequences())
    res.push_back(Database::SchemaItem(Role::sequence__,"sequence","CREATE SEQUENCE " + Role::sequence__ + " START 1 INCREMENT 1"));
    res.push_back(Database::SchemaItem(Student::table__,"table","CREATE TABLE " + Student::table__ + " (" + Student::Id.name() + " " + backend->getRowIDType() + ")"));
    res.push_back(Database::SchemaItem(Employee::table__,"table","CREATE TABLE " + Employee::table__ + " (" + Employee::Id.name() + " " + backend->getRowIDType() + ")"));
    res.push_back(Database::SchemaItem(School::table__,"table","CREATE TABLE " + School::table__ + " (" + School::Id.name() + " " + backend->getRowIDType() + "," + School::Type.name() + " " + School::Type.type() + "," + School::Name.name() + " " + School::Name.type() + ")"));
    if (backend->supportsSequences())
    res.push_back(Database::SchemaItem(School::sequence__,"sequence","CREATE SEQUENCE " + School::sequence__ + " START 1 INCREMENT 1"));
    res.push_back(Database::SchemaItem(Office::table__,"table","CREATE TABLE " + Office::table__ + " (" + Office::Id.name() + " " + backend->getRowIDType() + "," + Office::Type.name() + " " + Office::Type.type() + ")"));
    if (backend->supportsSequences())
    res.push_back(Database::SchemaItem(Office::sequence__,"sequence","CREATE SEQUENCE " + Office::sequence__ + " START 1 INCREMENT 1"));
    res.push_back(Database::SchemaItem(PersonPersonRelationMother::table__,"table","CREATE TABLE " + PersonPersonRelationMother::table__ + " (" + PersonPersonRelationMother::person1.name() + " " + PersonPersonRelationMother::person1.type() + " UNIQUE"+ "," + PersonPersonRelationMother::person2.name() + " " + PersonPersonRelationMother::person2.type() + ")"));
    res.push_back(Database::SchemaItem("Person_Person_Mother_all_idx","index","CREATE INDEX Person_Person_Mother_all_idx ON " + PersonPersonRelationMother::table__ + " (" + PersonPersonRelationMother::person1.name() + "," + PersonPersonRelationMother::person2.name() + ")"));
    res.push_back(Database::SchemaItem("_cdc62ca2d05d04aad6154853e5eeff0a","index","CREATE INDEX _cdc62ca2d05d04aad6154853e5eeff0a ON " + PersonPersonRelationMother::table__ + " (" + PersonPersonRelationMother::person1.name() + ")"));
    res.push_back(Database::SchemaItem("_19b685f391ae9a29eee2260423cb2e1c","index","CREATE INDEX _19b685f391ae9a29eee2260423cb2e1c ON " + PersonPersonRelationMother::table__ + " (" + PersonPersonRelationMother::person2.name() + ")"));
    res.push_back(Database::SchemaItem(PersonPersonRelationFather::table__,"table","CREATE TABLE " + PersonPersonRelationFather::table__ + " (" + PersonPersonRelationFather::person1.name() + " " + PersonPersonRelationFather::person1.type() + " UNIQUE"+ "," + PersonPersonRelationFather::person2.name() + " " + PersonPersonRelationFather::person2.type() + ")"));
    res.push_back(Database::SchemaItem("Person_Person_Father_all_idx","index","CREATE INDEX Person_Person_Father_all_idx ON " + PersonPersonRelationFather::table__ + " (" + PersonPersonRelationFather::person1.name() + "," + PersonPersonRelationFather::person2.name() + ")"));
    res.push_back(Database::SchemaItem("_2bcb3ead0dfff9e1086a524c6e4ad2c3","index","CREATE INDEX _2bcb3ead0dfff9e1086a524c6e4ad2c3 ON " + PersonPersonRelationFather::table__ + " (" + PersonPersonRelationFather::person1.name() + ")"));
    res.push_back(Database::SchemaItem("_7f31aa2d807b53cd7d4a4afc2d82f213","index","CREATE INDEX _7f31aa2d807b53cd7d4a4afc2d82f213 ON " + PersonPersonRelationFather::table__ + " (" + PersonPersonRelationFather::person2.name() + ")"));
    res.push_back(Database::SchemaItem(PersonPersonRelationSiblings::table__,"table","CREATE TABLE " + PersonPersonRelationSiblings::table__ + " (" + PersonPersonRelationSiblings::person1.name() + " " + PersonPersonRelationSiblings::person1.type()+ "," + PersonPersonRelationSiblings::person2.name() + " " + PersonPersonRelationSiblings::person2.type() + ")"));
    res.push_back(Database::SchemaItem("Person_Person_Siblings_all_idx","index","CREATE INDEX Person_Person_Siblings_all_idx ON " + PersonPersonRelationSiblings::table__ + " (" + PersonPersonRelationSiblings::person1.name() + "," + PersonPersonRelationSiblings::person2.name() + ")"));
    res.push_back(Database::SchemaItem("_40a9e165aea530226f7b3d11bbf6cc89","index","CREATE INDEX _40a9e165aea530226f7b3d11bbf6cc89 ON " + PersonPersonRelationSiblings::table__ + " (" + PersonPersonRelationSiblings::person1.name() + ")"));
    res.push_back(Database::SchemaItem("_d18823db577af84f812fec0813507d80","index","CREATE INDEX _d18823db577af84f812fec0813507d80 ON " + PersonPersonRelationSiblings::table__ + " (" + PersonPersonRelationSiblings::person2.name() + ")"));
    res.push_back(Database::SchemaItem(PersonPersonRelationChildren::table__,"table","CREATE TABLE " + PersonPersonRelationChildren::table__ + " (" + PersonPersonRelationChildren::person1.name() + " " + PersonPersonRelationChildren::person1.type()+ "," + PersonPersonRelationChildren::person2.name() + " " + PersonPersonRelationChildren::person2.type() + ")"));
    res.push_back(Database::SchemaItem("Person_Person_Children_all_idx","index","CREATE INDEX Person_Person_Children_all_idx ON " + PersonPersonRelationChildren::table__ + " (" + PersonPersonRelationChildren::person1.name() + "," + PersonPersonRelationChildren::person2.name() + ")"));
    res.push_back(Database::SchemaItem("_fe1eabc4d6da9afbc562dc04052d972d","index","CREATE INDEX _fe1eabc4d6da9afbc562dc04052d972d ON " + PersonPersonRelationChildren::table__ + " (" + PersonPersonRelationChildren::person1.name() + ")"));
    res.push_back(Database::SchemaItem("_81a57a61bb98b640be6eb729bb5b57f9","index","CREATE INDEX _81a57a61bb98b640be6eb729bb5b57f9 ON " + PersonPersonRelationChildren::table__ + " (" + PersonPersonRelationChildren::person2.name() + ")"));
    res.push_back(Database::SchemaItem(RoleRelation::table__,"table","CREATE TABLE " + RoleRelation::table__ + " (" + RoleRelation::person.name() + " " + RoleRelation::person.type()+ "," + RoleRelation::role.name() + " " + RoleRelation::role.type() + " UNIQUE" + ")"));
    res.push_back(Database::SchemaItem("Person_Role_Roles_all_idx","index","CREATE INDEX Person_Role_Roles_all_idx ON " + RoleRelation::table__ + " (" + RoleRelation::person.name() + "," + RoleRelation::role.name() + ")"));
    res.push_back(Database::SchemaItem("Person_Role_Roles_person_idx","index","CREATE INDEX Person_Role_Roles_person_idx ON " + RoleRelation::table__ + " (" + RoleRelation::person.name() + ")"));
    res.push_back(Database::SchemaItem("Person_Role_Roles_role_idx","index","CREATE INDEX Person_Role_Roles_role_idx ON " + RoleRelation::table__ + " (" + RoleRelation::role.name() + ")"));
    res.push_back(Database::SchemaItem(SchoolStudentRelation::table__,"table","CREATE TABLE " + SchoolStudentRelation::table__ + " (" + SchoolStudentRelation::school.name() + " " + SchoolStudentRelation::school.type()+ "," + SchoolStudentRelation::student.name() + " " + SchoolStudentRelation::student.type() + " UNIQUE" + ")"));
    res.push_back(Database::SchemaItem("School_Student__all_idx","index","CREATE INDEX School_Student__all_idx ON " + SchoolStudentRelation::table__ + " (" + SchoolStudentRelation::school.name() + "," + SchoolStudentRelation::student.name() + ")"));
    res.push_back(Database::SchemaItem("School_Student__school_idx","index","CREATE INDEX School_Student__school_idx ON " + SchoolStudentRelation::table__ + " (" + SchoolStudentRelation::school.name() + ")"));
    res.push_back(Database::SchemaItem("School_Student__student_idx","index","CREATE INDEX School_Student__student_idx ON " + SchoolStudentRelation::table__ + " (" + SchoolStudentRelation::student.name() + ")"));
    res.push_back(Database::SchemaItem(EmployeeOfficeRelation::table__,"table","CREATE TABLE " + EmployeeOfficeRelation::table__ + " (" + EmployeeOfficeRelation::employee.name() + " " + EmployeeOfficeRelation::employee.type()+ "," + EmployeeOfficeRelation::office.name() + " " + EmployeeOfficeRelation::office.type() + ")"));
    res.push_back(Database::SchemaItem("Employee_Office__all_idx","index","CREATE INDEX Employee_Office__all_idx ON " + EmployeeOfficeRelation::table__ + " (" + EmployeeOfficeRelation::employee.name() + "," + EmployeeOfficeRelation::office.name() + ")"));
    res.push_back(Database::SchemaItem("Employee_Office__employee_idx","index","CREATE INDEX Employee_Office__employee_idx ON " + EmployeeOfficeRelation::table__ + " (" + EmployeeOfficeRelation::employee.name() + ")"));
    res.push_back(Database::SchemaItem("Employee_Office__office_idx","index","CREATE INDEX Employee_Office__office_idx ON " + EmployeeOfficeRelation::table__ + " (" + EmployeeOfficeRelation::office.name() + ")"));
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

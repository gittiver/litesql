#ifndef _exampledatabase_hpp_
#define _exampledatabase_hpp_
#include "litesql.hpp"
namespace example {
class Person;
class Role;
class Student;
class Employee;
class School;
class Office;
class PersonPersonRelationMother;
class PersonPersonRelationFather;
class PersonPersonRelationSiblings;
class PersonPersonRelationChildren;
class RoleRelation;
class SchoolStudentRelation;
class EmployeeOfficeRelation;
class Expressions {
public:
    class Person;
    class Role;
    class School;
    class Office;
    class PersonPersonRelationMother;
    class PersonPersonRelationFather;
    class PersonPersonRelationSiblings;
    class PersonPersonRelationChildren;
    class RoleRelation;
    class SchoolStudentRelation;
    class EmployeeOfficeRelation;
    class Person : public litesql::Expr {
    public:
    protected:
        const std::string expr;
    public:
        inline virtual std::string asString() const {
            return expr;
        }
        inline Expressions::Person operator&&(const Expressions::Person& e) const {
            return Expressions::Person(litesql::And(litesql::RawExpr(expr), e));
        }
        inline Expressions::Person operator||(const Expressions::Person& e) const {
            return Expressions::Person(litesql::Or(litesql::RawExpr(expr), e));
        }
        Expressions::Person operator!() const;
        explicit inline Person(const litesql::Expr& e=litesql::Expr())
         : expr(e.asString()) {
        }
    };
    class Role : public litesql::Expr {
    public:
    protected:
        const std::string expr;
    public:
        inline virtual std::string asString() const {
            return expr;
        }
        inline Expressions::Role operator&&(const Expressions::Role& e) const {
            return Expressions::Role(litesql::And(litesql::RawExpr(expr), e));
        }
        inline Expressions::Role operator||(const Expressions::Role& e) const {
            return Expressions::Role(litesql::Or(litesql::RawExpr(expr), e));
        }
        Expressions::Role operator!() const;
        explicit inline Role(const litesql::Expr& e=litesql::Expr())
         : expr(e.asString()) {
        }
    };
    class School : public litesql::Expr {
    public:
    protected:
        const std::string expr;
    public:
        inline virtual std::string asString() const {
            return expr;
        }
        inline Expressions::School operator&&(const Expressions::School& e) const {
            return Expressions::School(litesql::And(litesql::RawExpr(expr), e));
        }
        inline Expressions::School operator||(const Expressions::School& e) const {
            return Expressions::School(litesql::Or(litesql::RawExpr(expr), e));
        }
        Expressions::School operator!() const;
        explicit inline School(const litesql::Expr& e=litesql::Expr())
         : expr(e.asString()) {
        }
    };
    class Office : public litesql::Expr {
    public:
    protected:
        const std::string expr;
    public:
        inline virtual std::string asString() const {
            return expr;
        }
        inline Expressions::Office operator&&(const Expressions::Office& e) const {
            return Expressions::Office(litesql::And(litesql::RawExpr(expr), e));
        }
        inline Expressions::Office operator||(const Expressions::Office& e) const {
            return Expressions::Office(litesql::Or(litesql::RawExpr(expr), e));
        }
        Expressions::Office operator!() const;
        explicit inline Office(const litesql::Expr& e=litesql::Expr())
         : expr(e.asString()) {
        }
    };
    class PersonPersonRelationMother : public litesql::Expr {
    public:
    protected:
        const std::string expr;
    public:
        inline virtual std::string asString() const {
            return expr;
        }
        inline Expressions::PersonPersonRelationMother operator&&(const Expressions::PersonPersonRelationMother& e) const {
            return Expressions::PersonPersonRelationMother(litesql::And(litesql::RawExpr(expr), e));
        }
        inline Expressions::PersonPersonRelationMother operator||(const Expressions::PersonPersonRelationMother& e) const {
            return Expressions::PersonPersonRelationMother(litesql::Or(litesql::RawExpr(expr), e));
        }
        Expressions::PersonPersonRelationMother operator!() const;
        explicit inline PersonPersonRelationMother(const litesql::Expr& e=litesql::Expr())
         : expr(e.asString()) {
        }
    };
    class PersonPersonRelationFather : public litesql::Expr {
    public:
    protected:
        const std::string expr;
    public:
        inline virtual std::string asString() const {
            return expr;
        }
        inline Expressions::PersonPersonRelationFather operator&&(const Expressions::PersonPersonRelationFather& e) const {
            return Expressions::PersonPersonRelationFather(litesql::And(litesql::RawExpr(expr), e));
        }
        inline Expressions::PersonPersonRelationFather operator||(const Expressions::PersonPersonRelationFather& e) const {
            return Expressions::PersonPersonRelationFather(litesql::Or(litesql::RawExpr(expr), e));
        }
        Expressions::PersonPersonRelationFather operator!() const;
        explicit inline PersonPersonRelationFather(const litesql::Expr& e=litesql::Expr())
         : expr(e.asString()) {
        }
    };
    class PersonPersonRelationSiblings : public litesql::Expr {
    public:
    protected:
        const std::string expr;
    public:
        inline virtual std::string asString() const {
            return expr;
        }
        inline Expressions::PersonPersonRelationSiblings operator&&(const Expressions::PersonPersonRelationSiblings& e) const {
            return Expressions::PersonPersonRelationSiblings(litesql::And(litesql::RawExpr(expr), e));
        }
        inline Expressions::PersonPersonRelationSiblings operator||(const Expressions::PersonPersonRelationSiblings& e) const {
            return Expressions::PersonPersonRelationSiblings(litesql::Or(litesql::RawExpr(expr), e));
        }
        Expressions::PersonPersonRelationSiblings operator!() const;
        explicit inline PersonPersonRelationSiblings(const litesql::Expr& e=litesql::Expr())
         : expr(e.asString()) {
        }
    };
    class PersonPersonRelationChildren : public litesql::Expr {
    public:
    protected:
        const std::string expr;
    public:
        inline virtual std::string asString() const {
            return expr;
        }
        inline Expressions::PersonPersonRelationChildren operator&&(const Expressions::PersonPersonRelationChildren& e) const {
            return Expressions::PersonPersonRelationChildren(litesql::And(litesql::RawExpr(expr), e));
        }
        inline Expressions::PersonPersonRelationChildren operator||(const Expressions::PersonPersonRelationChildren& e) const {
            return Expressions::PersonPersonRelationChildren(litesql::Or(litesql::RawExpr(expr), e));
        }
        Expressions::PersonPersonRelationChildren operator!() const;
        explicit inline PersonPersonRelationChildren(const litesql::Expr& e=litesql::Expr())
         : expr(e.asString()) {
        }
    };
    class RoleRelation : public litesql::Expr {
    public:
    protected:
        const std::string expr;
    public:
        inline virtual std::string asString() const {
            return expr;
        }
        inline Expressions::RoleRelation operator&&(const Expressions::RoleRelation& e) const {
            return Expressions::RoleRelation(litesql::And(litesql::RawExpr(expr), e));
        }
        inline Expressions::RoleRelation operator||(const Expressions::RoleRelation& e) const {
            return Expressions::RoleRelation(litesql::Or(litesql::RawExpr(expr), e));
        }
        Expressions::RoleRelation operator!() const;
        explicit inline RoleRelation(const litesql::Expr& e=litesql::Expr())
         : expr(e.asString()) {
        }
    };
    class SchoolStudentRelation : public litesql::Expr {
    public:
    protected:
        const std::string expr;
    public:
        inline virtual std::string asString() const {
            return expr;
        }
        inline Expressions::SchoolStudentRelation operator&&(const Expressions::SchoolStudentRelation& e) const {
            return Expressions::SchoolStudentRelation(litesql::And(litesql::RawExpr(expr), e));
        }
        inline Expressions::SchoolStudentRelation operator||(const Expressions::SchoolStudentRelation& e) const {
            return Expressions::SchoolStudentRelation(litesql::Or(litesql::RawExpr(expr), e));
        }
        Expressions::SchoolStudentRelation operator!() const;
        explicit inline SchoolStudentRelation(const litesql::Expr& e=litesql::Expr())
         : expr(e.asString()) {
        }
    };
    class EmployeeOfficeRelation : public litesql::Expr {
    public:
    protected:
        const std::string expr;
    public:
        inline virtual std::string asString() const {
            return expr;
        }
        inline Expressions::EmployeeOfficeRelation operator&&(const Expressions::EmployeeOfficeRelation& e) const {
            return Expressions::EmployeeOfficeRelation(litesql::And(litesql::RawExpr(expr), e));
        }
        inline Expressions::EmployeeOfficeRelation operator||(const Expressions::EmployeeOfficeRelation& e) const {
            return Expressions::EmployeeOfficeRelation(litesql::Or(litesql::RawExpr(expr), e));
        }
        Expressions::EmployeeOfficeRelation operator!() const;
        explicit inline EmployeeOfficeRelation(const litesql::Expr& e=litesql::Expr())
         : expr(e.asString()) {
        }
    };
};
class RowTypes {
public:
    class PersonPersonRelationMotherRow;
    class PersonPersonRelationFatherRow;
    class PersonPersonRelationSiblingsRow;
    class PersonPersonRelationChildrenRow;
    class RoleRelationRow;
    class SchoolStudentRelationRow;
    class EmployeeOfficeRelationRow;
    class PersonPersonRelationMotherRow {
    public:
        litesql::Field<int> person2;
        litesql::Field<int> person1;
        PersonPersonRelationMotherRow(const litesql::Database& db, const litesql::Record& rec=litesql::Record());
    };
    class PersonPersonRelationFatherRow {
    public:
        litesql::Field<int> person2;
        litesql::Field<int> person1;
        PersonPersonRelationFatherRow(const litesql::Database& db, const litesql::Record& rec=litesql::Record());
    };
    class PersonPersonRelationSiblingsRow {
    public:
        litesql::Field<int> person2;
        litesql::Field<int> person1;
        PersonPersonRelationSiblingsRow(const litesql::Database& db, const litesql::Record& rec=litesql::Record());
    };
    class PersonPersonRelationChildrenRow {
    public:
        litesql::Field<int> person2;
        litesql::Field<int> person1;
        PersonPersonRelationChildrenRow(const litesql::Database& db, const litesql::Record& rec=litesql::Record());
    };
    class RoleRelationRow {
    public:
        litesql::Field<int> role;
        litesql::Field<int> person;
        RoleRelationRow(const litesql::Database& db, const litesql::Record& rec=litesql::Record());
    };
    class SchoolStudentRelationRow {
    public:
        litesql::Field<int> student;
        litesql::Field<int> school;
        SchoolStudentRelationRow(const litesql::Database& db, const litesql::Record& rec=litesql::Record());
    };
    class EmployeeOfficeRelationRow {
    public:
        litesql::Field<int> office;
        litesql::Field<int> employee;
        EmployeeOfficeRelationRow(const litesql::Database& db, const litesql::Record& rec=litesql::Record());
    };
};
class PersonPersonRelationMother {
public:
    class FieldType;
    typedef Expressions::PersonPersonRelationMother Expr;
    typedef RowTypes::PersonPersonRelationMotherRow Row;
    class FieldType : public litesql::FieldType {
    public:
        template <class T> Expressions::PersonPersonRelationMother operator==(const T& value) const {
            return Expressions::PersonPersonRelationMother(litesql::Eq(*this, litesql::toString(value)));
        }
        template <class T> Expressions::PersonPersonRelationMother operator!=(const T& value) const {
            return Expressions::PersonPersonRelationMother(litesql::NotEq(*this, litesql::toString(value)));
        }
        template <class T> Expressions::PersonPersonRelationMother operator>(const T& value) const {
            return Expressions::PersonPersonRelationMother(litesql::Gt(*this, litesql::toString(value)));
        }
        template <class T> Expressions::PersonPersonRelationMother operator<(const T& value) const {
            return Expressions::PersonPersonRelationMother(litesql::Lt(*this, litesql::toString(value)));
        }
        template <class T> Expressions::PersonPersonRelationMother operator>=(const T& value) const {
            return Expressions::PersonPersonRelationMother(litesql::GtEq(*this, litesql::toString(value)));
        }
        template <class T> Expressions::PersonPersonRelationMother operator<=(const T& value) const {
            return Expressions::PersonPersonRelationMother(litesql::LtEq(*this, litesql::toString(value)));
        }
        template <class T> Expressions::PersonPersonRelationMother in(const T& value) const {
            return Expressions::PersonPersonRelationMother(litesql::In(*this, value));
        }
        template <class T> Expressions::PersonPersonRelationMother like(const T& value) const {
            return Expressions::PersonPersonRelationMother(litesql::Like(*this, litesql::toString(value)));
        }
        FieldType(const std::string& name, const std::string& type, const std::string& table, const Values& values=Values());
    };
    static const std::string table__;
    static const PersonPersonRelationMother::FieldType Person1;
    static const PersonPersonRelationMother::FieldType Person2;
    static void link(const litesql::Database& db, const example::Person& o0, const example::Person& o1);
    static void unlink(const litesql::Database& db, const example::Person& o0, const example::Person& o1);
    static void del(const litesql::Database& db, const PersonPersonRelationMother::Expr& expr=PersonPersonRelationMother::Expr());
    static litesql::DataSource<PersonPersonRelationMother::Row> getRows(const litesql::Database& db, const PersonPersonRelationMother::Expr& expr=PersonPersonRelationMother::Expr());
    static litesql::DataSource<example::Person> getPerson1(const litesql::Database& db, const Expressions::Person& expr=Expressions::Person(), const Expressions::PersonPersonRelationMother& srcExpr=Expressions::PersonPersonRelationMother());
    static litesql::DataSource<example::Person> getPerson2(const litesql::Database& db, const Expressions::Person& expr=Expressions::Person(), const Expressions::PersonPersonRelationMother& srcExpr=Expressions::PersonPersonRelationMother());
};
class PersonPersonRelationFather {
public:
    class FieldType;
    typedef Expressions::PersonPersonRelationFather Expr;
    typedef RowTypes::PersonPersonRelationFatherRow Row;
    class FieldType : public litesql::FieldType {
    public:
        template <class T> Expressions::PersonPersonRelationFather operator==(const T& value) const {
            return Expressions::PersonPersonRelationFather(litesql::Eq(*this, litesql::toString(value)));
        }
        template <class T> Expressions::PersonPersonRelationFather operator!=(const T& value) const {
            return Expressions::PersonPersonRelationFather(litesql::NotEq(*this, litesql::toString(value)));
        }
        template <class T> Expressions::PersonPersonRelationFather operator>(const T& value) const {
            return Expressions::PersonPersonRelationFather(litesql::Gt(*this, litesql::toString(value)));
        }
        template <class T> Expressions::PersonPersonRelationFather operator<(const T& value) const {
            return Expressions::PersonPersonRelationFather(litesql::Lt(*this, litesql::toString(value)));
        }
        template <class T> Expressions::PersonPersonRelationFather operator>=(const T& value) const {
            return Expressions::PersonPersonRelationFather(litesql::GtEq(*this, litesql::toString(value)));
        }
        template <class T> Expressions::PersonPersonRelationFather operator<=(const T& value) const {
            return Expressions::PersonPersonRelationFather(litesql::LtEq(*this, litesql::toString(value)));
        }
        template <class T> Expressions::PersonPersonRelationFather in(const T& value) const {
            return Expressions::PersonPersonRelationFather(litesql::In(*this, value));
        }
        template <class T> Expressions::PersonPersonRelationFather like(const T& value) const {
            return Expressions::PersonPersonRelationFather(litesql::Like(*this, litesql::toString(value)));
        }
        FieldType(const std::string& name, const std::string& type, const std::string& table, const Values& values=Values());
    };
    static const std::string table__;
    static const PersonPersonRelationFather::FieldType Person1;
    static const PersonPersonRelationFather::FieldType Person2;
    static void link(const litesql::Database& db, const example::Person& o0, const example::Person& o1);
    static void unlink(const litesql::Database& db, const example::Person& o0, const example::Person& o1);
    static void del(const litesql::Database& db, const PersonPersonRelationFather::Expr& expr=PersonPersonRelationFather::Expr());
    static litesql::DataSource<PersonPersonRelationFather::Row> getRows(const litesql::Database& db, const PersonPersonRelationFather::Expr& expr=PersonPersonRelationFather::Expr());
    static litesql::DataSource<example::Person> getPerson1(const litesql::Database& db, const Expressions::Person& expr=Expressions::Person(), const Expressions::PersonPersonRelationFather& srcExpr=Expressions::PersonPersonRelationFather());
    static litesql::DataSource<example::Person> getPerson2(const litesql::Database& db, const Expressions::Person& expr=Expressions::Person(), const Expressions::PersonPersonRelationFather& srcExpr=Expressions::PersonPersonRelationFather());
};
class PersonPersonRelationSiblings {
public:
    class FieldType;
    typedef Expressions::PersonPersonRelationSiblings Expr;
    typedef RowTypes::PersonPersonRelationSiblingsRow Row;
    class FieldType : public litesql::FieldType {
    public:
        template <class T> Expressions::PersonPersonRelationSiblings operator==(const T& value) const {
            return Expressions::PersonPersonRelationSiblings(litesql::Eq(*this, litesql::toString(value)));
        }
        template <class T> Expressions::PersonPersonRelationSiblings operator!=(const T& value) const {
            return Expressions::PersonPersonRelationSiblings(litesql::NotEq(*this, litesql::toString(value)));
        }
        template <class T> Expressions::PersonPersonRelationSiblings operator>(const T& value) const {
            return Expressions::PersonPersonRelationSiblings(litesql::Gt(*this, litesql::toString(value)));
        }
        template <class T> Expressions::PersonPersonRelationSiblings operator<(const T& value) const {
            return Expressions::PersonPersonRelationSiblings(litesql::Lt(*this, litesql::toString(value)));
        }
        template <class T> Expressions::PersonPersonRelationSiblings operator>=(const T& value) const {
            return Expressions::PersonPersonRelationSiblings(litesql::GtEq(*this, litesql::toString(value)));
        }
        template <class T> Expressions::PersonPersonRelationSiblings operator<=(const T& value) const {
            return Expressions::PersonPersonRelationSiblings(litesql::LtEq(*this, litesql::toString(value)));
        }
        template <class T> Expressions::PersonPersonRelationSiblings in(const T& value) const {
            return Expressions::PersonPersonRelationSiblings(litesql::In(*this, value));
        }
        template <class T> Expressions::PersonPersonRelationSiblings like(const T& value) const {
            return Expressions::PersonPersonRelationSiblings(litesql::Like(*this, litesql::toString(value)));
        }
        FieldType(const std::string& name, const std::string& type, const std::string& table, const Values& values=Values());
    };
    static const std::string table__;
    static const PersonPersonRelationSiblings::FieldType Person1;
    static const PersonPersonRelationSiblings::FieldType Person2;
    static void link(const litesql::Database& db, const example::Person& o0, const example::Person& o1);
    static void unlink(const litesql::Database& db, const example::Person& o0, const example::Person& o1);
    static void del(const litesql::Database& db, const PersonPersonRelationSiblings::Expr& expr=PersonPersonRelationSiblings::Expr());
    static litesql::DataSource<PersonPersonRelationSiblings::Row> getRows(const litesql::Database& db, const PersonPersonRelationSiblings::Expr& expr=PersonPersonRelationSiblings::Expr());
    static litesql::DataSource<example::Person> getPerson1(const litesql::Database& db, const Expressions::Person& expr=Expressions::Person(), const Expressions::PersonPersonRelationSiblings& srcExpr=Expressions::PersonPersonRelationSiblings());
    static litesql::DataSource<example::Person> getPerson2(const litesql::Database& db, const Expressions::Person& expr=Expressions::Person(), const Expressions::PersonPersonRelationSiblings& srcExpr=Expressions::PersonPersonRelationSiblings());
};
class PersonPersonRelationChildren {
public:
    class FieldType;
    typedef Expressions::PersonPersonRelationChildren Expr;
    typedef RowTypes::PersonPersonRelationChildrenRow Row;
    class FieldType : public litesql::FieldType {
    public:
        template <class T> Expressions::PersonPersonRelationChildren operator==(const T& value) const {
            return Expressions::PersonPersonRelationChildren(litesql::Eq(*this, litesql::toString(value)));
        }
        template <class T> Expressions::PersonPersonRelationChildren operator!=(const T& value) const {
            return Expressions::PersonPersonRelationChildren(litesql::NotEq(*this, litesql::toString(value)));
        }
        template <class T> Expressions::PersonPersonRelationChildren operator>(const T& value) const {
            return Expressions::PersonPersonRelationChildren(litesql::Gt(*this, litesql::toString(value)));
        }
        template <class T> Expressions::PersonPersonRelationChildren operator<(const T& value) const {
            return Expressions::PersonPersonRelationChildren(litesql::Lt(*this, litesql::toString(value)));
        }
        template <class T> Expressions::PersonPersonRelationChildren operator>=(const T& value) const {
            return Expressions::PersonPersonRelationChildren(litesql::GtEq(*this, litesql::toString(value)));
        }
        template <class T> Expressions::PersonPersonRelationChildren operator<=(const T& value) const {
            return Expressions::PersonPersonRelationChildren(litesql::LtEq(*this, litesql::toString(value)));
        }
        template <class T> Expressions::PersonPersonRelationChildren in(const T& value) const {
            return Expressions::PersonPersonRelationChildren(litesql::In(*this, value));
        }
        template <class T> Expressions::PersonPersonRelationChildren like(const T& value) const {
            return Expressions::PersonPersonRelationChildren(litesql::Like(*this, litesql::toString(value)));
        }
        FieldType(const std::string& name, const std::string& type, const std::string& table, const Values& values=Values());
    };
    static const std::string table__;
    static const PersonPersonRelationChildren::FieldType Person1;
    static const PersonPersonRelationChildren::FieldType Person2;
    static void link(const litesql::Database& db, const example::Person& o0, const example::Person& o1);
    static void unlink(const litesql::Database& db, const example::Person& o0, const example::Person& o1);
    static void del(const litesql::Database& db, const PersonPersonRelationChildren::Expr& expr=PersonPersonRelationChildren::Expr());
    static litesql::DataSource<PersonPersonRelationChildren::Row> getRows(const litesql::Database& db, const PersonPersonRelationChildren::Expr& expr=PersonPersonRelationChildren::Expr());
    static litesql::DataSource<example::Person> getPerson1(const litesql::Database& db, const Expressions::Person& expr=Expressions::Person(), const Expressions::PersonPersonRelationChildren& srcExpr=Expressions::PersonPersonRelationChildren());
    static litesql::DataSource<example::Person> getPerson2(const litesql::Database& db, const Expressions::Person& expr=Expressions::Person(), const Expressions::PersonPersonRelationChildren& srcExpr=Expressions::PersonPersonRelationChildren());
};
class RoleRelation {
public:
    class FieldType;
    typedef Expressions::RoleRelation Expr;
    typedef RowTypes::RoleRelationRow Row;
    class FieldType : public litesql::FieldType {
    public:
        template <class T> Expressions::RoleRelation operator==(const T& value) const {
            return Expressions::RoleRelation(litesql::Eq(*this, litesql::toString(value)));
        }
        template <class T> Expressions::RoleRelation operator!=(const T& value) const {
            return Expressions::RoleRelation(litesql::NotEq(*this, litesql::toString(value)));
        }
        template <class T> Expressions::RoleRelation operator>(const T& value) const {
            return Expressions::RoleRelation(litesql::Gt(*this, litesql::toString(value)));
        }
        template <class T> Expressions::RoleRelation operator<(const T& value) const {
            return Expressions::RoleRelation(litesql::Lt(*this, litesql::toString(value)));
        }
        template <class T> Expressions::RoleRelation operator>=(const T& value) const {
            return Expressions::RoleRelation(litesql::GtEq(*this, litesql::toString(value)));
        }
        template <class T> Expressions::RoleRelation operator<=(const T& value) const {
            return Expressions::RoleRelation(litesql::LtEq(*this, litesql::toString(value)));
        }
        template <class T> Expressions::RoleRelation in(const T& value) const {
            return Expressions::RoleRelation(litesql::In(*this, value));
        }
        template <class T> Expressions::RoleRelation like(const T& value) const {
            return Expressions::RoleRelation(litesql::Like(*this, litesql::toString(value)));
        }
        FieldType(const std::string& name, const std::string& type, const std::string& table, const Values& values=Values());
    };
    static const std::string table__;
    static const RoleRelation::FieldType Person;
    static const RoleRelation::FieldType Role;
    static void link(const litesql::Database& db, const example::Person& o0, const example::Role& o1);
    static void unlink(const litesql::Database& db, const example::Person& o0, const example::Role& o1);
    static void del(const litesql::Database& db, const RoleRelation::Expr& expr=RoleRelation::Expr());
    static litesql::DataSource<RoleRelation::Row> getRows(const litesql::Database& db, const RoleRelation::Expr& expr=RoleRelation::Expr());
    template <class T> static litesql::DataSource<T> get(const litesql::Database& db, const typename T::Expr& expr=typename T::Expr(), const Expressions::RoleRelation& srcExpr=Expressions::RoleRelation());
;
;
};
class SchoolStudentRelation {
public:
    class FieldType;
    typedef Expressions::SchoolStudentRelation Expr;
    typedef RowTypes::SchoolStudentRelationRow Row;
    class FieldType : public litesql::FieldType {
    public:
        template <class T> Expressions::SchoolStudentRelation operator==(const T& value) const {
            return Expressions::SchoolStudentRelation(litesql::Eq(*this, litesql::toString(value)));
        }
        template <class T> Expressions::SchoolStudentRelation operator!=(const T& value) const {
            return Expressions::SchoolStudentRelation(litesql::NotEq(*this, litesql::toString(value)));
        }
        template <class T> Expressions::SchoolStudentRelation operator>(const T& value) const {
            return Expressions::SchoolStudentRelation(litesql::Gt(*this, litesql::toString(value)));
        }
        template <class T> Expressions::SchoolStudentRelation operator<(const T& value) const {
            return Expressions::SchoolStudentRelation(litesql::Lt(*this, litesql::toString(value)));
        }
        template <class T> Expressions::SchoolStudentRelation operator>=(const T& value) const {
            return Expressions::SchoolStudentRelation(litesql::GtEq(*this, litesql::toString(value)));
        }
        template <class T> Expressions::SchoolStudentRelation operator<=(const T& value) const {
            return Expressions::SchoolStudentRelation(litesql::LtEq(*this, litesql::toString(value)));
        }
        template <class T> Expressions::SchoolStudentRelation in(const T& value) const {
            return Expressions::SchoolStudentRelation(litesql::In(*this, value));
        }
        template <class T> Expressions::SchoolStudentRelation like(const T& value) const {
            return Expressions::SchoolStudentRelation(litesql::Like(*this, litesql::toString(value)));
        }
        FieldType(const std::string& name, const std::string& type, const std::string& table, const Values& values=Values());
    };
    static const std::string table__;
    static const SchoolStudentRelation::FieldType School;
    static const SchoolStudentRelation::FieldType Student;
    static void link(const litesql::Database& db, const example::School& o0, const example::Student& o1);
    static void unlink(const litesql::Database& db, const example::School& o0, const example::Student& o1);
    static void del(const litesql::Database& db, const SchoolStudentRelation::Expr& expr=SchoolStudentRelation::Expr());
    static litesql::DataSource<SchoolStudentRelation::Row> getRows(const litesql::Database& db, const SchoolStudentRelation::Expr& expr=SchoolStudentRelation::Expr());
    template <class T> static litesql::DataSource<T> get(const litesql::Database& db, const typename T::Expr& expr=typename T::Expr(), const Expressions::SchoolStudentRelation& srcExpr=Expressions::SchoolStudentRelation());
;
;
};
class EmployeeOfficeRelation {
public:
    class FieldType;
    typedef Expressions::EmployeeOfficeRelation Expr;
    typedef RowTypes::EmployeeOfficeRelationRow Row;
    class FieldType : public litesql::FieldType {
    public:
        template <class T> Expressions::EmployeeOfficeRelation operator==(const T& value) const {
            return Expressions::EmployeeOfficeRelation(litesql::Eq(*this, litesql::toString(value)));
        }
        template <class T> Expressions::EmployeeOfficeRelation operator!=(const T& value) const {
            return Expressions::EmployeeOfficeRelation(litesql::NotEq(*this, litesql::toString(value)));
        }
        template <class T> Expressions::EmployeeOfficeRelation operator>(const T& value) const {
            return Expressions::EmployeeOfficeRelation(litesql::Gt(*this, litesql::toString(value)));
        }
        template <class T> Expressions::EmployeeOfficeRelation operator<(const T& value) const {
            return Expressions::EmployeeOfficeRelation(litesql::Lt(*this, litesql::toString(value)));
        }
        template <class T> Expressions::EmployeeOfficeRelation operator>=(const T& value) const {
            return Expressions::EmployeeOfficeRelation(litesql::GtEq(*this, litesql::toString(value)));
        }
        template <class T> Expressions::EmployeeOfficeRelation operator<=(const T& value) const {
            return Expressions::EmployeeOfficeRelation(litesql::LtEq(*this, litesql::toString(value)));
        }
        template <class T> Expressions::EmployeeOfficeRelation in(const T& value) const {
            return Expressions::EmployeeOfficeRelation(litesql::In(*this, value));
        }
        template <class T> Expressions::EmployeeOfficeRelation like(const T& value) const {
            return Expressions::EmployeeOfficeRelation(litesql::Like(*this, litesql::toString(value)));
        }
        FieldType(const std::string& name, const std::string& type, const std::string& table, const Values& values=Values());
    };
    static const std::string table__;
    static const EmployeeOfficeRelation::FieldType Employee;
    static const EmployeeOfficeRelation::FieldType Office;
    static void link(const litesql::Database& db, const example::Employee& o0, const example::Office& o1);
    static void unlink(const litesql::Database& db, const example::Employee& o0, const example::Office& o1);
    static void del(const litesql::Database& db, const EmployeeOfficeRelation::Expr& expr=EmployeeOfficeRelation::Expr());
    static litesql::DataSource<EmployeeOfficeRelation::Row> getRows(const litesql::Database& db, const EmployeeOfficeRelation::Expr& expr=EmployeeOfficeRelation::Expr());
    template <class T> static litesql::DataSource<T> get(const litesql::Database& db, const typename T::Expr& expr=typename T::Expr(), const Expressions::EmployeeOfficeRelation& srcExpr=Expressions::EmployeeOfficeRelation());
;
;
};
class Person : public litesql::Persistent {
public:
    class FieldType;
    class Own;
    class Sex;
    class MotherHandle;
    class FatherHandle;
    class SiblingsHandle;
    class ChildrenHandle;
    class RolesHandle;
    typedef Expressions::Person Expr;
    class FieldType : public litesql::FieldType {
    public:
        template <class T> Expressions::Person operator==(const T& value) const {
            return Expressions::Person(litesql::Eq(*this, litesql::toString(value)));
        }
        template <class T> Expressions::Person operator!=(const T& value) const {
            return Expressions::Person(litesql::NotEq(*this, litesql::toString(value)));
        }
        template <class T> Expressions::Person operator>(const T& value) const {
            return Expressions::Person(litesql::Gt(*this, litesql::toString(value)));
        }
        template <class T> Expressions::Person operator<(const T& value) const {
            return Expressions::Person(litesql::Lt(*this, litesql::toString(value)));
        }
        template <class T> Expressions::Person operator>=(const T& value) const {
            return Expressions::Person(litesql::GtEq(*this, litesql::toString(value)));
        }
        template <class T> Expressions::Person operator<=(const T& value) const {
            return Expressions::Person(litesql::LtEq(*this, litesql::toString(value)));
        }
        template <class T> Expressions::Person in(const T& value) const {
            return Expressions::Person(litesql::In(*this, value));
        }
        template <class T> Expressions::Person like(const T& value) const {
            return Expressions::Person(litesql::Like(*this, litesql::toString(value)));
        }
        FieldType(const std::string& name, const std::string& type, const std::string& table, const Values& values=Values());
    };
    class Own {
    public:
        static const Person::FieldType Id;
    };
    class Sex {
    public:
        static const int Male;
        static const int Female;
    };
    class MotherHandle : public litesql::RelationHandle<Person> {
    public:
        MotherHandle(const Person& owner);
        void link(const Person& o0);
        void unlink(const Person& o0);
        void del(const Expressions::PersonPersonRelationMother& srcExpr=Expressions::PersonPersonRelationMother());
        litesql::DataSource<Person> get(const Expressions::Person& expr=Expressions::Person(), const Expressions::PersonPersonRelationMother& srcExpr=Expressions::PersonPersonRelationMother());
        litesql::DataSource<RowTypes::PersonPersonRelationMotherRow> getRows(const Expressions::PersonPersonRelationMother& srcExpr=Expressions::PersonPersonRelationMother());
    };
    class FatherHandle : public litesql::RelationHandle<Person> {
    public:
        FatherHandle(const Person& owner);
        void link(const Person& o0);
        void unlink(const Person& o0);
        void del(const Expressions::PersonPersonRelationFather& srcExpr=Expressions::PersonPersonRelationFather());
        litesql::DataSource<Person> get(const Expressions::Person& expr=Expressions::Person(), const Expressions::PersonPersonRelationFather& srcExpr=Expressions::PersonPersonRelationFather());
        litesql::DataSource<RowTypes::PersonPersonRelationFatherRow> getRows(const Expressions::PersonPersonRelationFather& srcExpr=Expressions::PersonPersonRelationFather());
    };
    class SiblingsHandle : public litesql::RelationHandle<Person> {
    public:
        SiblingsHandle(const Person& owner);
        void link(const Person& o0);
        void unlink(const Person& o0);
        void del(const Expressions::PersonPersonRelationSiblings& srcExpr=Expressions::PersonPersonRelationSiblings());
        litesql::DataSource<Person> get(const Expressions::Person& expr=Expressions::Person(), const Expressions::PersonPersonRelationSiblings& srcExpr=Expressions::PersonPersonRelationSiblings());
        litesql::DataSource<RowTypes::PersonPersonRelationSiblingsRow> getRows(const Expressions::PersonPersonRelationSiblings& srcExpr=Expressions::PersonPersonRelationSiblings());
    };
    class ChildrenHandle : public litesql::RelationHandle<Person> {
    public:
        ChildrenHandle(const Person& owner);
        void link(const Person& o0);
        void unlink(const Person& o0);
        void del(const Expressions::PersonPersonRelationChildren& srcExpr=Expressions::PersonPersonRelationChildren());
        litesql::DataSource<Person> get(const Expressions::Person& expr=Expressions::Person(), const Expressions::PersonPersonRelationChildren& srcExpr=Expressions::PersonPersonRelationChildren());
        litesql::DataSource<RowTypes::PersonPersonRelationChildrenRow> getRows(const Expressions::PersonPersonRelationChildren& srcExpr=Expressions::PersonPersonRelationChildren());
    };
    class RolesHandle : public litesql::RelationHandle<Person> {
    public:
        RolesHandle(const Person& owner);
        void link(const Role& o0);
        void unlink(const Role& o0);
        void del(const Expressions::RoleRelation& srcExpr=Expressions::RoleRelation());
        litesql::DataSource<Role> get(const Expressions::Role& expr=Expressions::Role(), const Expressions::RoleRelation& srcExpr=Expressions::RoleRelation());
        litesql::DataSource<RowTypes::RoleRelationRow> getRows(const Expressions::RoleRelation& srcExpr=Expressions::RoleRelation());
    };
    static const std::string type__;
    static const std::string table__;
    static const std::string sequence__;
    static const Person::FieldType Id;
    litesql::Field<int> id;
    static const Person::FieldType Type;
    litesql::Field<std::string> type;
    static const Person::FieldType Name;
    litesql::Field<std::string> name;
    static const Person::FieldType Age;
    litesql::Field<int> age;
    static const Person::FieldType Pubdate;
    litesql::Field<litesql::Date> pubdate;
protected:
    static std::vector < std::pair< std::string, std::string > > sex_values;
public:
    static const Person::FieldType Sex;
    litesql::Field<int> sex;
    static void initValues();
protected:
    void defaults();
public:
    Person(const litesql::Database& db);
    Person(const litesql::Database& db, const litesql::Record& rec);
    Person(const Person& obj);
    const Person& operator=(const Person& obj);
    Person::MotherHandle mother() const;
    Person::FatherHandle father() const;
    Person::SiblingsHandle siblings() const;
    Person::ChildrenHandle children() const;
    Person::RolesHandle roles() const;
    virtual void sayHello();
protected:
    std::string insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs);
    void create();
    virtual void addUpdates(Updates& updates);
    virtual void addIDUpdates(Updates& updates);
public:
    static void getFieldTypes(std::vector<litesql::FieldType>& ftypes);
protected:
    virtual void delRecord();
    virtual void delRelations();
public:
    virtual void update();
    virtual void del();
    virtual bool typeIsCorrect();
    std::auto_ptr<Person> upcast();
    std::auto_ptr<Person> upcastCopy();
};
std::ostream & operator<<(std::ostream& os, Person o);
class Role : public litesql::Persistent {
public:
    class FieldType;
    class Own;
    class PersonHandle;
    typedef Expressions::Role Expr;
    class FieldType : public litesql::FieldType {
    public:
        template <class T> Expressions::Role operator==(const T& value) const {
            return Expressions::Role(litesql::Eq(*this, litesql::toString(value)));
        }
        template <class T> Expressions::Role operator!=(const T& value) const {
            return Expressions::Role(litesql::NotEq(*this, litesql::toString(value)));
        }
        template <class T> Expressions::Role operator>(const T& value) const {
            return Expressions::Role(litesql::Gt(*this, litesql::toString(value)));
        }
        template <class T> Expressions::Role operator<(const T& value) const {
            return Expressions::Role(litesql::Lt(*this, litesql::toString(value)));
        }
        template <class T> Expressions::Role operator>=(const T& value) const {
            return Expressions::Role(litesql::GtEq(*this, litesql::toString(value)));
        }
        template <class T> Expressions::Role operator<=(const T& value) const {
            return Expressions::Role(litesql::LtEq(*this, litesql::toString(value)));
        }
        template <class T> Expressions::Role in(const T& value) const {
            return Expressions::Role(litesql::In(*this, value));
        }
        template <class T> Expressions::Role like(const T& value) const {
            return Expressions::Role(litesql::Like(*this, litesql::toString(value)));
        }
        FieldType(const std::string& name, const std::string& type, const std::string& table, const Values& values=Values());
    };
    class Own {
    public:
        static const Role::FieldType Id;
    };
    class PersonHandle : public litesql::RelationHandle<Role> {
    public:
        PersonHandle(const Role& owner);
        void link(const Person& o0);
        void unlink(const Person& o0);
        void del(const Expressions::RoleRelation& srcExpr=Expressions::RoleRelation());
        litesql::DataSource<Person> get(const Expressions::Person& expr=Expressions::Person(), const Expressions::RoleRelation& srcExpr=Expressions::RoleRelation());
        litesql::DataSource<RowTypes::RoleRelationRow> getRows(const Expressions::RoleRelation& srcExpr=Expressions::RoleRelation());
    };
    static const std::string type__;
    static const std::string table__;
    static const std::string sequence__;
    static const Role::FieldType Id;
    litesql::Field<int> id;
    static const Role::FieldType Type;
    litesql::Field<std::string> type;
protected:
    void defaults();
public:
    Role(const litesql::Database& db);
    Role(const litesql::Database& db, const litesql::Record& rec);
    Role(const Role& obj);
    const Role& operator=(const Role& obj);
    Role::PersonHandle person() const;
protected:
    std::string insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs);
    void create();
    virtual void addUpdates(Updates& updates);
    virtual void addIDUpdates(Updates& updates);
public:
    static void getFieldTypes(std::vector<litesql::FieldType>& ftypes);
protected:
    virtual void delRecord();
    virtual void delRelations();
public:
    virtual void update();
    virtual void del();
    virtual bool typeIsCorrect();
    std::auto_ptr<Role> upcast();
    std::auto_ptr<Role> upcastCopy();
};
std::ostream & operator<<(std::ostream& os, Role o);
class Student : public Role {
public:
    class Own;
    class SchoolHandle;
    class Own {
    public:
        static const Role::FieldType Id;
    };
    class SchoolHandle : public litesql::RelationHandle<Student> {
    public:
        SchoolHandle(const Student& owner);
        void link(const School& o0);
        void unlink(const School& o0);
        void del(const Expressions::SchoolStudentRelation& srcExpr=Expressions::SchoolStudentRelation());
        litesql::DataSource<School> get(const Expressions::School& expr=Expressions::School(), const Expressions::SchoolStudentRelation& srcExpr=Expressions::SchoolStudentRelation());
        litesql::DataSource<RowTypes::SchoolStudentRelationRow> getRows(const Expressions::SchoolStudentRelation& srcExpr=Expressions::SchoolStudentRelation());
    };
    static const std::string type__;
    static const std::string table__;
    Student(const litesql::Database& db);
    Student(const litesql::Database& db, const litesql::Record& rec);
    Student(const Student& obj);
    const Student& operator=(const Student& obj);
    Student::SchoolHandle school() const;
protected:
    std::string insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs);
    void create();
    virtual void addUpdates(Updates& updates);
    virtual void addIDUpdates(Updates& updates);
public:
    static void getFieldTypes(std::vector<litesql::FieldType>& ftypes);
protected:
    virtual void delRecord();
    virtual void delRelations();
public:
    virtual void update();
    virtual void del();
    virtual bool typeIsCorrect();
    std::auto_ptr<Student> upcast();
    std::auto_ptr<Student> upcastCopy();
};
std::ostream & operator<<(std::ostream& os, Student o);
class Employee : public Role {
public:
    class Own;
    class OfficeHandle;
    class Own {
    public:
        static const Role::FieldType Id;
    };
    class OfficeHandle : public litesql::RelationHandle<Employee> {
    public:
        OfficeHandle(const Employee& owner);
        void link(const Office& o0);
        void unlink(const Office& o0);
        void del(const Expressions::EmployeeOfficeRelation& srcExpr=Expressions::EmployeeOfficeRelation());
        litesql::DataSource<Office> get(const Expressions::Office& expr=Expressions::Office(), const Expressions::EmployeeOfficeRelation& srcExpr=Expressions::EmployeeOfficeRelation());
        litesql::DataSource<RowTypes::EmployeeOfficeRelationRow> getRows(const Expressions::EmployeeOfficeRelation& srcExpr=Expressions::EmployeeOfficeRelation());
    };
    static const std::string type__;
    static const std::string table__;
    Employee(const litesql::Database& db);
    Employee(const litesql::Database& db, const litesql::Record& rec);
    Employee(const Employee& obj);
    const Employee& operator=(const Employee& obj);
    Employee::OfficeHandle office() const;
protected:
    std::string insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs);
    void create();
    virtual void addUpdates(Updates& updates);
    virtual void addIDUpdates(Updates& updates);
public:
    static void getFieldTypes(std::vector<litesql::FieldType>& ftypes);
protected:
    virtual void delRecord();
    virtual void delRelations();
public:
    virtual void update();
    virtual void del();
    virtual bool typeIsCorrect();
    std::auto_ptr<Employee> upcast();
    std::auto_ptr<Employee> upcastCopy();
};
std::ostream & operator<<(std::ostream& os, Employee o);
class School : public litesql::Persistent {
public:
    class FieldType;
    class Own;
    class StudentsHandle;
    typedef Expressions::School Expr;
    class FieldType : public litesql::FieldType {
    public:
        template <class T> Expressions::School operator==(const T& value) const {
            return Expressions::School(litesql::Eq(*this, litesql::toString(value)));
        }
        template <class T> Expressions::School operator!=(const T& value) const {
            return Expressions::School(litesql::NotEq(*this, litesql::toString(value)));
        }
        template <class T> Expressions::School operator>(const T& value) const {
            return Expressions::School(litesql::Gt(*this, litesql::toString(value)));
        }
        template <class T> Expressions::School operator<(const T& value) const {
            return Expressions::School(litesql::Lt(*this, litesql::toString(value)));
        }
        template <class T> Expressions::School operator>=(const T& value) const {
            return Expressions::School(litesql::GtEq(*this, litesql::toString(value)));
        }
        template <class T> Expressions::School operator<=(const T& value) const {
            return Expressions::School(litesql::LtEq(*this, litesql::toString(value)));
        }
        template <class T> Expressions::School in(const T& value) const {
            return Expressions::School(litesql::In(*this, value));
        }
        template <class T> Expressions::School like(const T& value) const {
            return Expressions::School(litesql::Like(*this, litesql::toString(value)));
        }
        FieldType(const std::string& name, const std::string& type, const std::string& table, const Values& values=Values());
    };
    class Own {
    public:
        static const School::FieldType Id;
    };
    class StudentsHandle : public litesql::RelationHandle<School> {
    public:
        StudentsHandle(const School& owner);
        void link(const Student& o0);
        void unlink(const Student& o0);
        void del(const Expressions::SchoolStudentRelation& srcExpr=Expressions::SchoolStudentRelation());
        litesql::DataSource<Student> get(const Expressions::Role& expr=Expressions::Role(), const Expressions::SchoolStudentRelation& srcExpr=Expressions::SchoolStudentRelation());
        litesql::DataSource<RowTypes::SchoolStudentRelationRow> getRows(const Expressions::SchoolStudentRelation& srcExpr=Expressions::SchoolStudentRelation());
    };
    static const std::string type__;
    static const std::string table__;
    static const std::string sequence__;
    static const School::FieldType Id;
    litesql::Field<int> id;
    static const School::FieldType Type;
    litesql::Field<std::string> type;
    static const School::FieldType Name;
    litesql::Field<std::string> name;
protected:
    void defaults();
public:
    School(const litesql::Database& db);
    School(const litesql::Database& db, const litesql::Record& rec);
    School(const School& obj);
    const School& operator=(const School& obj);
    School::StudentsHandle students() const;
protected:
    std::string insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs);
    void create();
    virtual void addUpdates(Updates& updates);
    virtual void addIDUpdates(Updates& updates);
public:
    static void getFieldTypes(std::vector<litesql::FieldType>& ftypes);
protected:
    virtual void delRecord();
    virtual void delRelations();
public:
    virtual void update();
    virtual void del();
    virtual bool typeIsCorrect();
    std::auto_ptr<School> upcast();
    std::auto_ptr<School> upcastCopy();
};
std::ostream & operator<<(std::ostream& os, School o);
class Office : public litesql::Persistent {
public:
    class FieldType;
    class Own;
    class EmployeesHandle;
    typedef Expressions::Office Expr;
    class FieldType : public litesql::FieldType {
    public:
        template <class T> Expressions::Office operator==(const T& value) const {
            return Expressions::Office(litesql::Eq(*this, litesql::toString(value)));
        }
        template <class T> Expressions::Office operator!=(const T& value) const {
            return Expressions::Office(litesql::NotEq(*this, litesql::toString(value)));
        }
        template <class T> Expressions::Office operator>(const T& value) const {
            return Expressions::Office(litesql::Gt(*this, litesql::toString(value)));
        }
        template <class T> Expressions::Office operator<(const T& value) const {
            return Expressions::Office(litesql::Lt(*this, litesql::toString(value)));
        }
        template <class T> Expressions::Office operator>=(const T& value) const {
            return Expressions::Office(litesql::GtEq(*this, litesql::toString(value)));
        }
        template <class T> Expressions::Office operator<=(const T& value) const {
            return Expressions::Office(litesql::LtEq(*this, litesql::toString(value)));
        }
        template <class T> Expressions::Office in(const T& value) const {
            return Expressions::Office(litesql::In(*this, value));
        }
        template <class T> Expressions::Office like(const T& value) const {
            return Expressions::Office(litesql::Like(*this, litesql::toString(value)));
        }
        FieldType(const std::string& name, const std::string& type, const std::string& table, const Values& values=Values());
    };
    class Own {
    public:
        static const Office::FieldType Id;
    };
    class EmployeesHandle : public litesql::RelationHandle<Office> {
    public:
        EmployeesHandle(const Office& owner);
        void link(const Employee& o0);
        void unlink(const Employee& o0);
        void del(const Expressions::EmployeeOfficeRelation& srcExpr=Expressions::EmployeeOfficeRelation());
        litesql::DataSource<Employee> get(const Expressions::Role& expr=Expressions::Role(), const Expressions::EmployeeOfficeRelation& srcExpr=Expressions::EmployeeOfficeRelation());
        litesql::DataSource<RowTypes::EmployeeOfficeRelationRow> getRows(const Expressions::EmployeeOfficeRelation& srcExpr=Expressions::EmployeeOfficeRelation());
    };
    static const std::string type__;
    static const std::string table__;
    static const std::string sequence__;
    static const Office::FieldType Id;
    litesql::Field<int> id;
    static const Office::FieldType Type;
    litesql::Field<std::string> type;
protected:
    void defaults();
public:
    Office(const litesql::Database& db);
    Office(const litesql::Database& db, const litesql::Record& rec);
    Office(const Office& obj);
    const Office& operator=(const Office& obj);
    Office::EmployeesHandle employees() const;
protected:
    std::string insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs);
    void create();
    virtual void addUpdates(Updates& updates);
    virtual void addIDUpdates(Updates& updates);
public:
    static void getFieldTypes(std::vector<litesql::FieldType>& ftypes);
protected:
    virtual void delRecord();
    virtual void delRelations();
public:
    virtual void update();
    virtual void del();
    virtual bool typeIsCorrect();
    std::auto_ptr<Office> upcast();
    std::auto_ptr<Office> upcastCopy();
};
std::ostream & operator<<(std::ostream& os, Office o);
class ExampleDatabase : public litesql::Database {
public:
    ExampleDatabase(std::string backendType, std::string connInfo);
protected:
    virtual std::vector<litesql::Database::SchemaItem> getSchema() const;
    static void initialize();
};
}
#endif

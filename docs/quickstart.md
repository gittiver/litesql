# Quick Start {#quickstart}

# Step 1: Write XML-file which defines objects, fields, methods and relations in database.

Simple example database definition file exampledatabase.xml: 
~~~~~~~~~~~~~~~{.xml}
<?xml version="1.0"?>
<!DOCTYPE database SYSTEM "litesql.dtd"> 
<database name="ExampleDatabase" namespace="example">
    <object name="Person">
        <field name="name" type="string"/>
        <field name="age" type="integer" default="15"/>
        <field name="sex" type="integer">
            <value name="Male" value="0"/>
            <value name="Female" value="1"/>
        </field>
        <method name="sayHello"/>
    </object>
    <relation id="Mother" unidir="true">
        <relate object="Person" limit="many" handle="mother"/>
        <relate object="Person" limit="one"/>
    </relation>
    <relation id="Father" unidir="true">
        <relate object="Person" limit="many" handle="father"/>
        <relate object="Person" limit="one"/>
    </relation>
    <relation id="Siblings">
        <relate object="Person" handle="siblings"/>
        <relate object="Person"/>
    </relation>
    <relation id="Children" unidir="true">
        <relate object="Person" handle="children"/>
        <relate object="Person"/>
    </relation>
    <object name="Role"/>
    <object name="Student" inherits="Role"/>
    <object name="Employee" inherits="Role"/>
    <relation id="Roles" name="RoleRelation">
        <relate object="Person" handle="roles" limit="one"/>
        <relate object="Role" handle="person"/>
    </relation>
    <object name="School">
        <field name="name" type="string"/>
    </object>
    <relation>
        <relate object="Student" handle="school"/>
        <relate object="School" handle="students" limit="one"/>
    </relation>
    <object name="Office"/>
    <relation>
        <relate object="Employee" handle="office"/>
        <relate object="Office" handle="employees"/>
    </relation>
</database>
~~~~~~~~~~~~~~~

Figure of database (generated with graphviz and litesql-gen):

![image](exampledatabase.png)

# Step 2: Run Code Generator

~~~~~~~~~~~~~~~{.sh}
litesql-gen -t c++ exampledatabase.xml
~~~~~~~~~~~~~~~

Files exampledatabase.cpp and exampledatabase.hpp are written.

# Step 3: Write Application Code

~~~~~~~~~~~~~~~{.c}
	// include LiteSQL's header file and generated header file
#include <iostream>
#include "litesql.hpp"
#include "exampledatabase.hpp"
// provide implementation for Person::sayHello
void example::Person::sayHello() {
    std::cout << "Hi! My name is " << name 
        << " and I am " << age << " years old." << std::endl;
}

// no name collisions expected
using namespace litesql;
using namespace example;

int main(int argc, char **argv) {

         std::cout << "joo" << std::endl;
    try {
        // using SQLite3 as backend
         ExampleDatabase db("sqlite3", "database=example.db");
        // create tables, sequences and indexes
        db.verbose = true;
        db.create();
        // start transaction
        db.begin();

        // create couple of Person-objects
        Person jeff(db);
        jeff.name = "Jeff";
        jeff.sex = Person::Sex::Male;
        jeff.age = 32;
        // store Jeff to database
        jeff.update();
        Person jill(db);
        jill.name = "Jill";
        jill.sex = Person::Sex::Female;
        jill.age = 33;
        jill.update();
        Person jack(db);
        jack.name = "Jack";
        jack.sex = Person::Sex::Male;
        jack.update();
        Person jess(db);
        jess.name = "Jess";
        jess.sex = Person::Sex::Female;
        jess.update();
        // build up relationships between Persons 
        jeff.children().link(jack);
        jill.children().link(jack);
        jill.children().link(jess);
        jack.father().link(jeff);
        jack.mother().link(jill);
        jess.mother().link(jill);
        jack.siblings().link(jill);
        // roles (linking examples)
        Office office(db);
        office.update();
        School school(db);
        school.update();

        Employee jeffRole(db);
        jeffRole.update();
        jeff.roles().link(jeffRole);
        jeffRole.office().link(office);

        Student jackRole(db), jessRole(db);
        jackRole.update();
        jessRole.update();
        jack.roles().link(jackRole);
        jess.roles().link(jessRole);
        
        jackRole.school().link(school);
        jessRole.school().link(school);
        
        // count Persons
        cout << "There are " << select<Person>(db).count() 
             << " persons." << endl;
    
        // select all Persons and order them by age
        vector<Person> family = select<Person>(db).orderBy(Person::Age).all();
        // show results
        for (vector<Person>::iterator i = family.begin(); i != family.end(); i++)
            cout << toString(*i) << endl;
             
        // select intersection of Jeff's and Jill's children and
        // iterate results with cursor
        Cursor<Person> cursor = intersect(jeff.children().get(), 
                                          jill.children().get()).cursor();
        // Jack should say hello
        for (;cursor.rowsLeft();cursor++) 
            (*cursor).sayHello();
        
        // select a non-existing Person
        try {
            select<Person>(db, Person::Id == 100).one();
        } catch (NotFound e) {
            cout << "No Person with id 100" << endl;
        }
        // commit transaction
        db.commit();
        // clean up 
        db.drop();
    } catch (Except e) {
        cerr << e << endl;
        return -1;
    }
    return 0;
}
~~~~~~~~~~~~~~~


# Step 4: Compile, Link and Execute ##

When executed, the example program will output following:
~~~~~~~~~~~~~~~{.sh}
There are 4 persons.
-------------------------------------
id_ = 4
type_ = Person
name_ = Jess
age_ = 15
sex_ = 1
-------------------------------------

-------------------------------------
id_ = 3
type_ = Person
name_ = Jack
age_ = 15
sex_ = 0
-------------------------------------

-------------------------------------
id_ = 1
type_ = Person
name_ = Jeff
age_ = 32
sex_ = 0
-------------------------------------

-------------------------------------
id_ = 2
type_ = Person
name_ = Jill
age_ = 33
sex_ = 1
-------------------------------------

Hi! My name is Jack and I am 15 years old.
No Person with id 100
~~~~~~~~~~~~~~~

Following SQL queries are executed under the hood. Most of the queries are related to creating database schema and storing it to schema-table.

~~~~~~~~~~~~~~~{.sql}
CREATE TABLE schema (name TEXT, type TEXT, sql TEXT);
DELETE FROM schema WHERE name='schema' and type='table'
INSERT INTO schema VALUES ('schema','table','CREATE TABLE schema (name TEXT, 
type TEXT, sql TEXT);')
CREATE TABLE Person_ (id_ INTEGER PRIMARY KEY,type_ TEXT,name_ TEXT,age_ 
INTEGER,sex_ INTEGER)
DELETE FROM schema WHERE name='Person_' and type='table'
INSERT INTO schema VALUES ('Person_','table','CREATE TABLE Person_ (id_ INTEGER 
PRIMARY KEY,type_ TEXT,name_ TEXT,age_ INTEGER,sex_ INTEGER)')
CREATE TABLE Role_ (id_ INTEGER PRIMARY KEY,type_ TEXT)
DELETE FROM schema WHERE name='Role_' and type='table'
INSERT INTO schema VALUES ('Role_','table','CREATE TABLE Role_ (id_ INTEGER 
PRIMARY KEY,type_ TEXT)')
CREATE TABLE Student_ (id_ INTEGER PRIMARY KEY)
DELETE FROM schema WHERE name='Student_' and type='table'
INSERT INTO schema VALUES ('Student_','table','CREATE TABLE Student_ (id_ 
INTEGER PRIMARY KEY)')
CREATE TABLE Employee_ (id_ INTEGER PRIMARY KEY)
DELETE FROM schema WHERE name='Employee_' and type='table'
INSERT INTO schema VALUES ('Employee_','table','CREATE TABLE Employee_ (id_ 
INTEGER PRIMARY KEY)')
CREATE TABLE School_ (id_ INTEGER PRIMARY KEY,type_ TEXT,name_ TEXT)
DELETE FROM schema WHERE name='School_' and type='table'
INSERT INTO schema VALUES ('School_','table','CREATE TABLE School_ (id_ INTEGER 
PRIMARY KEY,type_ TEXT,name_ TEXT)')
CREATE TABLE Office_ (id_ INTEGER PRIMARY KEY,type_ TEXT)
DELETE FROM schema WHERE name='Office_' and type='table'
INSERT INTO schema VALUES ('Office_','table','CREATE TABLE Office_ (id_ INTEGER 
PRIMARY KEY,type_ TEXT)')
CREATE TABLE Person_Person_Mother (Person1 INTEGER UNIQUE,Person2 INTEGER)
DELETE FROM schema WHERE name='Person_Person_Mother' and type='table'
INSERT INTO schema VALUES ('Person_Person_Mother','table','CREATE TABLE 
Person_Person_Mother (Person1 INTEGER UNIQUE,Person2 INTEGER)')
CREATE INDEX Person_Person_Mother_all_idx ON Person_Person_Mother 
(Person1,Person2)
DELETE FROM schema WHERE name='Person_Person_Mother_all_idx' and type='index'
INSERT INTO schema VALUES ('Person_Person_Mother_all_idx','index','CREATE INDEX 
Person_Person_Mother_all_idx ON Person_Person_Mother (Person1,Person2)')
CREATE INDEX _cdc62ca2d05d04aad6154853e5eeff0a ON Person_Person_Mother (Person1)
DELETE FROM schema WHERE name='_cdc62ca2d05d04aad6154853e5eeff0a' and 
type='index'
INSERT INTO schema VALUES ('_cdc62ca2d05d04aad6154853e5eeff0a','index','CREATE 
INDEX _cdc62ca2d05d04aad6154853e5eeff0a ON Person_Person_Mother (Person1)')
CREATE INDEX _19b685f391ae9a29eee2260423cb2e1c ON Person_Person_Mother (Person2)
DELETE FROM schema WHERE name='_19b685f391ae9a29eee2260423cb2e1c' and 
type='index'
INSERT INTO schema VALUES ('_19b685f391ae9a29eee2260423cb2e1c','index','CREATE 
INDEX _19b685f391ae9a29eee2260423cb2e1c ON Person_Person_Mother (Person2)')
CREATE TABLE Person_Person_Father (Person1 INTEGER UNIQUE,Person2 INTEGER)
DELETE FROM schema WHERE name='Person_Person_Father' and type='table'
INSERT INTO schema VALUES ('Person_Person_Father','table','CREATE TABLE 
Person_Person_Father (Person1 INTEGER UNIQUE,Person2 INTEGER)')
CREATE INDEX Person_Person_Father_all_idx ON Person_Person_Father 
(Person1,Person2)
DELETE FROM schema WHERE name='Person_Person_Father_all_idx' and type='index'
INSERT INTO schema VALUES ('Person_Person_Father_all_idx','index','CREATE INDEX 
Person_Person_Father_all_idx ON Person_Person_Father (Person1,Person2)')
CREATE INDEX _2bcb3ead0dfff9e1086a524c6e4ad2c3 ON Person_Person_Father (Person1)
DELETE FROM schema WHERE name='_2bcb3ead0dfff9e1086a524c6e4ad2c3' and 
type='index'
INSERT INTO schema VALUES ('_2bcb3ead0dfff9e1086a524c6e4ad2c3','index','CREATE 
INDEX _2bcb3ead0dfff9e1086a524c6e4ad2c3 ON Person_Person_Father (Person1)')
CREATE INDEX _7f31aa2d807b53cd7d4a4afc2d82f213 ON Person_Person_Father (Person2)
DELETE FROM schema WHERE name='_7f31aa2d807b53cd7d4a4afc2d82f213' and 
type='index'
INSERT INTO schema VALUES ('_7f31aa2d807b53cd7d4a4afc2d82f213','index','CREATE 
INDEX _7f31aa2d807b53cd7d4a4afc2d82f213 ON Person_Person_Father (Person2)')
CREATE TABLE Person_Person_Siblings (Person1 INTEGER,Person2 INTEGER)
DELETE FROM schema WHERE name='Person_Person_Siblings' and type='table'
INSERT INTO schema VALUES ('Person_Person_Siblings','table','CREATE TABLE 
Person_Person_Siblings (Person1 INTEGER,Person2 INTEGER)')
CREATE INDEX Person_Person_Siblings_all_idx ON Person_Person_Siblings 
(Person1,Person2)
DELETE FROM schema WHERE name='Person_Person_Siblings_all_idx' and type='index'
INSERT INTO schema VALUES ('Person_Person_Siblings_all_idx','index','CREATE 
INDEX Person_Person_Siblings_all_idx ON Person_Person_Siblings 
(Person1,Person2)')
CREATE INDEX _40a9e165aea530226f7b3d11bbf6cc89 ON Person_Person_Siblings 
(Person1)
DELETE FROM schema WHERE name='_40a9e165aea530226f7b3d11bbf6cc89' and 
type='index'
INSERT INTO schema VALUES ('_40a9e165aea530226f7b3d11bbf6cc89','index','CREATE 
INDEX _40a9e165aea530226f7b3d11bbf6cc89 ON Person_Person_Siblings (Person1)')
CREATE INDEX _d18823db577af84f812fec0813507d80 ON Person_Person_Siblings 
(Person2)
DELETE FROM schema WHERE name='_d18823db577af84f812fec0813507d80' and 
type='index'
INSERT INTO schema VALUES ('_d18823db577af84f812fec0813507d80','index','CREATE 
INDEX _d18823db577af84f812fec0813507d80 ON Person_Person_Siblings (Person2)')
CREATE TABLE Person_Person_Children (Person1 INTEGER,Person2 INTEGER)
DELETE FROM schema WHERE name='Person_Person_Children' and type='table'
INSERT INTO schema VALUES ('Person_Person_Children','table','CREATE TABLE 
Person_Person_Children (Person1 INTEGER,Person2 INTEGER)')
CREATE INDEX Person_Person_Children_all_idx ON Person_Person_Children 
(Person1,Person2)
DELETE FROM schema WHERE name='Person_Person_Children_all_idx' and type='index'
INSERT INTO schema VALUES ('Person_Person_Children_all_idx','index','CREATE 
INDEX Person_Person_Children_all_idx ON Person_Person_Children 
(Person1,Person2)')
CREATE INDEX _fe1eabc4d6da9afbc562dc04052d972d ON Person_Person_Children 
(Person1)
DELETE FROM schema WHERE name='_fe1eabc4d6da9afbc562dc04052d972d' and 
type='index'
INSERT INTO schema VALUES ('_fe1eabc4d6da9afbc562dc04052d972d','index','CREATE 
INDEX _fe1eabc4d6da9afbc562dc04052d972d ON Person_Person_Children (Person1)')
CREATE INDEX _81a57a61bb98b640be6eb729bb5b57f9 ON Person_Person_Children 
(Person2)
DELETE FROM schema WHERE name='_81a57a61bb98b640be6eb729bb5b57f9' and 
type='index'
INSERT INTO schema VALUES ('_81a57a61bb98b640be6eb729bb5b57f9','index','CREATE 
INDEX _81a57a61bb98b640be6eb729bb5b57f9 ON Person_Person_Children (Person2)')
CREATE TABLE Person_Role_Roles (Person1 INTEGER,Role2 INTEGER UNIQUE)
DELETE FROM schema WHERE name='Person_Role_Roles' and type='table'
INSERT INTO schema VALUES ('Person_Role_Roles','table','CREATE TABLE 
Person_Role_Roles (Person1 INTEGER,Role2 INTEGER UNIQUE)')
CREATE INDEX Person_Role_Roles_all_idx ON Person_Role_Roles (Person1,Role2)
DELETE FROM schema WHERE name='Person_Role_Roles_all_idx' and type='index'
INSERT INTO schema VALUES ('Person_Role_Roles_all_idx','index','CREATE INDEX 
Person_Role_Roles_all_idx ON Person_Role_Roles (Person1,Role2)')
CREATE INDEX Person_Role_Roles_person_idx ON Person_Role_Roles (Person1)
DELETE FROM schema WHERE name='Person_Role_Roles_person_idx' and type='index'
INSERT INTO schema VALUES ('Person_Role_Roles_person_idx','index','CREATE INDEX 
Person_Role_Roles_person_idx ON Person_Role_Roles (Person1)')
CREATE INDEX Person_Role_Roles_role_idx ON Person_Role_Roles (Role2)
DELETE FROM schema WHERE name='Person_Role_Roles_role_idx' and type='index'
INSERT INTO schema VALUES ('Person_Role_Roles_role_idx','index','CREATE INDEX 
Person_Role_Roles_role_idx ON Person_Role_Roles (Role2)')
CREATE TABLE School_Student_ (School1 INTEGER,Student2 INTEGER UNIQUE)
DELETE FROM schema WHERE name='School_Student_' and type='table'
INSERT INTO schema VALUES ('School_Student_','table','CREATE TABLE 
School_Student_ (School1 INTEGER,Student2 INTEGER UNIQUE)')
CREATE INDEX School_Student__all_idx ON School_Student_ (School1,Student2)
DELETE FROM schema WHERE name='School_Student__all_idx' and type='index'
INSERT INTO schema VALUES ('School_Student__all_idx','index','CREATE INDEX 
School_Student__all_idx ON School_Student_ (School1,Student2)')
CREATE INDEX School_Student__school_idx ON School_Student_ (School1)
DELETE FROM schema WHERE name='School_Student__school_idx' and type='index'
INSERT INTO schema VALUES ('School_Student__school_idx','index','CREATE INDEX 
School_Student__school_idx ON School_Student_ (School1)')
CREATE INDEX School_Student__student_idx ON School_Student_ (Student2)
DELETE FROM schema WHERE name='School_Student__student_idx' and type='index'
INSERT INTO schema VALUES ('School_Student__student_idx','index','CREATE INDEX 
School_Student__student_idx ON School_Student_ (Student2)')
CREATE TABLE Employee_Office_ (Employee1 INTEGER,Office2 INTEGER)
DELETE FROM schema WHERE name='Employee_Office_' and type='table'
INSERT INTO schema VALUES ('Employee_Office_','table','CREATE TABLE 
Employee_Office_ (Employee1 INTEGER,Office2 INTEGER)')
CREATE INDEX Employee_Office__all_idx ON Employee_Office_ (Employee1,Office2)
DELETE FROM schema WHERE name='Employee_Office__all_idx' and type='index'
INSERT INTO schema VALUES ('Employee_Office__all_idx','index','CREATE INDEX 
Employee_Office__all_idx ON Employee_Office_ (Employee1,Office2)')
CREATE INDEX Employee_Office__employee_idx ON Employee_Office_ (Employee1)
DELETE FROM schema WHERE name='Employee_Office__employee_idx' and type='index'
INSERT INTO schema VALUES ('Employee_Office__employee_idx','index','CREATE 
INDEX Employee_Office__employee_idx ON Employee_Office_ (Employee1)')
CREATE INDEX Employee_Office__office_idx ON Employee_Office_ (Office2)
DELETE FROM schema WHERE name='Employee_Office__office_idx' and type='index'
INSERT INTO schema VALUES ('Employee_Office__office_idx','index','CREATE INDEX 
Employee_Office__office_idx ON Employee_Office_ (Office2)')
INSERT INTO Person_ (id_,type_,name_,age_,sex_) VALUES 
(NULL,'Person','Jeff','32','0');
INSERT INTO Person_ (id_,type_,name_,age_,sex_) VALUES 
(NULL,'Person','Jill','33','1');
INSERT INTO Person_ (id_,type_,name_,age_,sex_) VALUES 
(NULL,'Person','Jack','15','0');
INSERT INTO Person_ (id_,type_,name_,age_,sex_) VALUES 
(NULL,'Person','Jess','15','1');
INSERT INTO Person_Person_Children (Person1,Person2) VALUES ('1','3')
INSERT INTO Person_Person_Children (Person1,Person2) VALUES ('2','3')
INSERT INTO Person_Person_Children (Person1,Person2) VALUES ('2','4')
INSERT INTO Person_Person_Father (Person1,Person2) VALUES ('3','1')
INSERT INTO Person_Person_Mother (Person1,Person2) VALUES ('3','2')
INSERT INTO Person_Person_Mother (Person1,Person2) VALUES ('2','2')
INSERT INTO Person_Person_Siblings (Person1,Person2) VALUES ('3','2')
INSERT INTO Person_Person_Siblings (Person1,Person2) VALUES ('2','3')
INSERT INTO Role_ (id_,type_) values (NULL,'Employee');
INSERT INTO Employee_ (id_) values (1);
INSERT INTO Person_Role_Roles (Person1,Role2) VALUES ('1','1')
INSERT INTO Employee_Office_ (Employee1,Office2) VALUES ('1','1')
INSERT INTO Role_ (id_,type_) values (NULL,'Student');
INSERT INTO Student_ (id_) values (2);
INSERT INTO Role_ (id_,type_) values (NULL,'Student');
INSERT INTO Student_ (id_) values (3);
INSERT INTO School_ (id_,type_) values (NULL, 'School');
INSERT INTO Office_ (id_,type_) values (NULL, 'School');
INSERT INTO Person_Role_Roles (Person1,Role2) VALUES ('3','2')
INSERT INTO Person_Role_Roles (Person1,Role2) VALUES ('2','3')
INSERT INTO School_Student_ (School1,Student2) VALUES ('1','2')
INSERT INTO School_Student_ (School1,Student2) VALUES ('1','3')
SELECT count(*) FROM Person_
SELECT Person_.id_,Person_.type_,Person_.name_,Person_.age_,Person_.sex_ FROM 
Person_ ORDER BY Person_.age_
SELECT Person_.id_,Person_.type_,Person_.name_,Person_.age_,Person_.sex_ FROM 
Person_ WHERE Person_.id_ in (SELECT Person_.id_ FROM Person_ WHERE Person_.id_ 
in (SELECT Person_Person_Children.Person2 FROM Person_Person_Children WHERE 
Person_Person_Children.Person1 = '1') INTERSECT SELECT Person_.id_ FROM Person_ 
WHERE Person_.id_ in (SELECT Person_Person_Children.Person2 FROM 
Person_Person_Children WHERE Person_Person_Children.Person1 = '2'))
SELECT Person_.id_,Person_.type_,Person_.name_,Person_.age_,Person_.sex_ FROM 
Person_ WHERE Person_.id_ = '100'
DROP TABLE schema
DROP TABLE Person_
DROP TABLE Role_
DROP TABLE Student_
DROP TABLE Employee_
DROP TABLE School_
DROP TABLE Office_
DROP TABLE Person_Person_Mother
DROP TABLE Person_Person_Father
DROP TABLE Person_Person_Siblings
DROP TABLE Person_Person_Children
DROP TABLE Person_Role_Roles
DROP TABLE School_Student_
DROP TABLE Employee_Office_
~~~~~~~~~~~~~~~


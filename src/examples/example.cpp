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
    try {
        // using SQLite3 as backend
        ExampleDatabase db("sqlite3", "database=example.db");
        // create tables, sequences and indexes
        db.create();
        char buf[100];
        gets(buf);
        db.query("select * from Person_");

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
        jill.mother().link(jill);
        jack.siblings().link(jill);
        // select all Persons and order them by age
        vector<Person> family = select<Person>(db).orderBy(Person::age_).all();
        // show results
        for (vector<Person>::iterator i = family.begin(); i != family.end(); i++)
            cout << toString(*i) << endl;
             
        // select intersection of Jeff's and Jill's children and
        // iterate results with cursor
        family = jeff.children().get().all();
        for (vector<Person>::iterator i = family.begin(); i != family.end(); i++)
            cout << toString(*i) << endl;

        Cursor<Person> cursor = intersect(jeff.children().get(), 
                                          jill.children().get()).cursor();
        // Jack should say hello
        for (;cursor.rowsLeft();cursor++) 
            (*cursor).sayHello();
        
        // select a non-existing Person
        try {
            select<Person>(db, Person::id_ == 100).one();
        } catch (NotFound e) {
            cout << "No Person with id 100" << endl;
        }
        // clean up 
        db.drop();
    } catch (Except e) {
        cerr << e << endl;
        return -1;
    }
    return 0;
}

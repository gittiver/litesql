Examples
====================================================

# Creating and Dropping Database

The code below defines a simple database:

~~~~~~~~~~~~~~~{.xml}

<?xml version="1.0"?>
<!DOCTYPE database SYSTEM "litesql.dtd">
<database name="PersonDatabase">
    <object name="Person">
        <field name="name" type="string"/>
    </object>
</database>
~~~~~~~~~~~~~~~

The database file is created when PersonDatabase is constructed. First argument of constructor is requested backend. Other possibilities are "postgresql" and "mysql". Refer to class documentation for details.
~~~~~~~~~~~~~~~{.cpp}

PersonDatabase db("sqlite3", "database=person.db");
~~~~~~~~~~~~~~~

The database has to be created before any Persistent - classes can be used.
~~~~~~~~~~~~~~~{.cpp}

db.create();
~~~~~~~~~~~~~~~

The database can be dropped using drop() - method.
~~~~~~~~~~~~~~~{.cpp}
db.drop();
~~~~~~~~~~~~~~~

Suppose the database schema has changed along with new features of the program. Few data fields has been added to Person-class and completely new Persistent-class House has been defined. To upgrade database on disk, use needsUpgrade() and upgrade() - methods.
~~~~~~~~~~~~~~~{.cpp}

if (db.needsUpgrade())
    db.upgrade();
~~~~~~~~~~~~~~~

If new fields has been added to classes, the upgrade operation may be quite costly. All backends do not support table modifications so records has to be copied forth and back.

Upgrade routine will try to preserve data. It will drop fields that are not in new schema and insert NULL-values to new fields. 

# Storing and Deleting Persistents #

A simple Person database:
~~~~~~~~~~~~~~~{.xml}

<?xml version="1.0"?>
<!DOCTYPE database SYSTEM "litesql.dtd">
<database name="PersonDatabase">
    <object name="Person">
        <field name="name" type="string"/>
        <field name="age" type="integer"/>
    </object>
</database>
~~~~~~~~~~~~~~~

Following code demonstrates how to insert Persons to database:
~~~~~~~~~~~~~~~{.cpp}

PersonDatabase db("sqlite3", "database=person.db"); // assumes the database has been created

Person person(db); // construct Person, does not write anything to database
person.name = "Bob"; // assign values to fields
person.age = 20;
person.update(); // writes a new record to database

person.age = 21; // Bob got just older
person.update(); // updates old record

person.id = 100; // force internal identifier (id) to 100 
person.update(); // updates old record
~~~~~~~~~~~~~~~

Note: if internal identifier (id-field) is changed, relations will not "follow" the object and will not be deleted either. If the object is not replaced with another object, relations should be manually dropped using delRelations-method.

Following code demonstrates how to delete Persons from database:
~~~~~~~~~~~~~~~{.cpp}

Person person = select<Person>(db).one(); // any Person will do

person.del(); // person.onDelete() gets called before data is deleted
~~~~~~~~~~~~~~~

# Selecting Persistents and Expr-API #

## select-function ##

~~~~~~~~~~~~~~~{.cpp}

template <class T> select(const Database& db, const Expr& filter=Expr());
~~~~~~~~~~~~~~~

Template function select<T> returns DataSource which can be used to retrieve Persistent-objects or to create more complex queries.

select-function has two parameters: Database and expression (Expr) which can be used to limit the result set.
DataSource
DataSource<T> is basically a wrapper for selection query and is used to access objects of type T.

DataSource<T> has following methods:

    * idQuery(): returns SelectQuery which returns id-number of Persistent objects
    * objectQuery(): returns SelectQuery which returns data rows of Persistent objects
    * cursor(): returns Cursor<T> which will return Persistent objects one by one
    * one(): returns first Persistent object in the result set. Note: throws NotFound - if the result set is empty
    * all(): returns the whole result set as Persistent objects in vector
    * count(): returns number of object in the result set
    * orderBy(FieldType field, bool ascending=true): result set can be ordered with this method.
    * orderByRelation(FieldType id, FieldType field, bool ascending=true): orders the result set by external relation

## Selection Examples ##

An example of select and selectOne with database:
~~~~~~~~~~~~~~~{.cpp}

PersonDatabase db("sqlite3", "database=person.db");
vector<Person> = select<Person>(db).all();

Person bob = select<Person>(db, Person::Name == "Bob").one();
~~~~~~~~~~~~~~~

SelectQuery-class can be used when creating SQL SELECT-statements. Refer to documentation of SelectQuery for a list of methods. Methods can be combined:
~~~~~~~~~~~~~~~{.cpp}

Records recs = db.query(SelectQuery().result("id_")
                                     .source("Person_")
                                     .where(Person::Name == "Bob"));
~~~~~~~~~~~~~~~

## Selection expressions (filters) ##

Filters for select are created using Expr-API. The simplest form of a filter expression is <field> <operator> <value>. Example:
~~~~~~~~~~~~~~~{.cpp}

Person::Name == "Bob"
~~~~~~~~~~~~~~~

These expression can be combined using parenthesis and connectives:
~~~~~~~~~~~~~~~{.cpp}

() && ||
~~~~~~~~~~~~~~~

Operators that can be used in expressions are
~~~~~~~~~~~~~~~{.cpp}
== > < >= <= ! != .in .like
~~~~~~~~~~~~~~~

Persistent's (static) field information can be accessed using static FieldType-objects The name of the FieldType-object is almost the same as the field. Capitalize name of the field to get name of the FieldType-object.
~~~~~~~~~~~~~~~{.cpp}

// Person's name
Person::Name
~~~~~~~~~~~~~~~

## Set-operations intersect, union_ and except ##

~~~~~~~~~~~~~~~{.cpp}

template <class T> intersect(const DataSource<T>& ds1,
                             const DataSource<T>& ds2);
template <class T> union_(const DataSource<T>& ds1,
                          const DataSource<T>& ds2);
template <class T> except(const DataSource<T>& ds1,
                          const DataSource<T>& ds2);
~~~~~~~~~~~~~~~

Two DataSources of same type can be used to create a new DataSource which can be used to access intersection, union or difference of the sources.

Notes: -'union' is a reserved word of C so union_ is the name of set operation. -see MySQL specific notes about using these set operations with MySQL
~~~~~~~~~~~~~~~{.cpp}

// intersection of Bob's and Bill's friends
DataSource<Person> commonFriends = 
    intersect(bob.friends().get(), bill.friends().get());

// persons that are Bob's friends and/or Bill's friends
DataSource<Person> allFriends = 
    union_(bob.friends().get(), bill.friends().get());

// persons that are Bob's friends but not Bill's friends
DataSource<Person> allFriends = 
    except(bob.friends().get(), bill.friends().get());
~~~~~~~~~~~~~~~

# Using Relations

A simple database with Person-class and friends-relation.
~~~~~~~~~~~~~~~{.xml}

<?xml version="1.0"?>
<!DOCTYPE database SYSTEM "http://litesql.sourceforge.net/litesql.dtd">
<database name="TestDatabase" namespace="test">
    <object name="Person">
        <field name="name" type="string"/>
    </object>
    <relation name="FriendsRelation">
        <relate object="Person" handle="friends"/>
        <relate object="Person"/>
    </relation>
</database>    
~~~~~~~~~~~~~~~

## Relation-class ##

Usually relations are accessed using a relation handle that is attached to a persistent object. Sometimes, it may be convenient to access relation using static methods of Relation-class.

Methods:
 * link : link objects
 * unlink : remove a link between objects
 * get<type> : get a DataSource<type> of objects
 * del : drop links using an expression
 * ( getTYPEn : non-template versions for relations with duplicate types )

## RelationHandle-class ##
Relation handle is attached to persistent object and it provides convenient access to relation. It can be used to link, unlink or select objects related to relation handle's owner.

RelationHandle-class' methods:
 * link : link owner to another object
 * unlink : unlink owner from another object
 * del : drop links using an expression
 * get<type> : get a DataSource<type> of objects (generated for n-ary relations)
 * get : get a DataSource of known type (generated for 2-ended relations, only one type)
 * ( getTYPEn : non-template versions for relations with three or more duplicate types )

## RelationHandle-Examples ##

A linking example:
~~~~~~~~~~~~~~~{.cpp}

Person bill(db), bob(db);
bill.name = "Bill";
bill.update();

bob.name = "Bob";
bob.update();

// both objects must be stored in database before they can be linked

bill.friends().link(bob);

// following statement would throw an exception because they are already friends
// (friends is bidirectional relation)
bob.friends().link(bill);
~~~~~~~~~~~~~~~

A fetching example:
~~~~~~~~~~~~~~~{.cpp}

Person bob = bill.friends().get(Person::Name == "Bob").one();

vector<Person> billsFriends = bill.friends().get().all();
~~~~~~~~~~~~~~~

An unlinking example:
~~~~~~~~~~~~~~~{.cpp}

// Bill and Bob are no longer friends
bill.friends().unlink(bob);
~~~~~~~~~~~~~~~Relation-Examples
Same examples as above converted to static methods of FriendsRelation.

A linking example:
~~~~~~~~~~~~~~~{.cpp}

Person bill(db), bob(db);
bill.name = "Bill";
bill.update();

bob.name = "Bob";
bob.update();

// both objects must be stored in database before they can be linked

FriendsRelation::link(db, bill, bob);
~~~~~~~~~~~~~~~

A fetching example:
~~~~~~~~~~~~~~~{.cpp}

Person bob = FriendsRelation::getPerson2(db, 
                                         Person::Name == "Bob",
                                         FriendsRelation::Person1==bill.id).one();

vector<Person> billsFriends = 
     FriendsRelation::getPerson2(db, Expr(), 
                                 FriendsRelation::Person1==bill.id).all();
~~~~~~~~~~~~~~~

An unlinking example:
~~~~~~~~~~~~~~~{.cpp}

// Bill and Bob are no longer friends
Friendsrelation::unlink(bill, bob);
~~~~~~~~~~~~~~~

# Upcasting and Virtual Methods #

In order to support virtual methods a Persistent object must be upcasted to correct type before calling virtual method.

upcast will return an auto_ptr which holds a pointer to an object of correct type.

upcastCopy is similar to upcast. It does not select an object of correct type from database but just constructs it in memory. Note that constructed object does not have all of its fields. This method is suitable when virtual method to be called does not need any special fields.

An example of upcast:
~~~~~~~~~~~~~~~{.cpp}

Playable pl = select<Playable>(db).one();
pl.upcast()->play();
~~~~~~~~~~~~~~~

An another example of upcast:
~~~~~~~~~~~~~~~{.cpp}

Playable pl = select<Playable>(db).one();
auto_ptr<Playable> uc = pl.upcast();
cout << "Playing " << uc->name << endl;
uc->play();
~~~~~~~~~~~~~~~

An example of upcastCopy:
~~~~~~~~~~~~~~~{.cpp}

Playable pl = select<Playable>(db).one();
pl.upcastCopy()->makeLogEntry(log);
~~~~~~~~~~~~~~~

# Using Cursor (iterating the result set row by row)

Often it is not necessary to fetch all objects to memory and then process them.

Memory usage can be minimized using template class Cursor which returns one object at a time.

Creating a cursor(two ways):
~~~~~~~~~~~~~~~{.cpp}

Cursor<Record> cur1 = db.cursor(SelectQuery().result("id_").source("Person_"));
Cursor<Person> cur2 = select<Person>(db).cursor();
~~~~~~~~~~~~~~~

Iterating through the result set:
~~~~~~~~~~~~~~~{.cpp}

for (;cur.rowsLeft(); cur++)
    cout << (*cur).name << endl;
~~~~~~~~~~~~~~~

The remaining contents of the result set can be retrieved with dump-method:
~~~~~~~~~~~~~~~{.cpp}

vector<Person> rest = cur.dump();
~~~~~~~~~~~~~~~

# Database API: raw queries

If you prefer to access database using raw queries, use plain Database-class and query-method.

escapeSQL-function can be used to escape illegal characters such as '. NULL is a special word and it has to be escaped by the user. 'NULL' maps to NULL and ''NULL'' to 'NULL' etc.

Use Database::groupInsert - if you want to create rows with new integer as the first field.

A query example:
~~~~~~~~~~~~~~~{.cpp}

Database db("sqlite3", "database=test.db");
db.query("CREATE TABLE custom(value INTEGER);");
db.query("INSERT INTO custom VALUES (1)");
Records recs = db.query("SELECT * from custom");
for (Records::iterator i = recs.begin(); i != recs.end(); i++)
    cout << (*i)[0] << endl;
~~~~~~~~~~~~~~~

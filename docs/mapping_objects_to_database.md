How Objects Are Stored in the Database {#mapping_objects_to_database}
======================================

Please refer to [Scott Ambler's article](http://www.agiledata.org/essays/mappingObjects.html) for detailed information about object-relational mapping techniques. This document states briefly how LiteSQL does it.

Persistent Objects
==================

A persistent object is an instance of a class that has a possibly empty predefined set of data attributes. A data attribute here is a labelled atomic piece of data. 

The most intuitive way to store this kind of data in a relational database is to model each class as a table and instances of a class as rows in the corresponding table. Persistent objects that are instances of derived classes are possibly composed of several sets of data attributes. Each of these sets is stored in a separate table. 

In C++, an instance of a class is located at a specific location of memory. However, if only the data attributes of an instance were stored in the database, different instances with the values same int their data attributes could not be distinguished. That is why each persistent object is assigned a numeric object identifier which is unique within the class hierarchy ie. (derived) instances of different base classes may share object identifiers.

Summary:
 * one table per one class (including derived classes)
 * one column in a table per one data attribute
 * extra column in every table to identify objects == object identifier (integer)
 * instances of derived classes are split into rows in multiple tables

More About Object Identifiers
=============================

Persistent objects can be instantiated in two ways (in C++). Either they are created as new or they are constructed using data from the database. In either case, the persistent object is given a database reference. If the persistent object was created as new, it does not have an object identifier because there are no corresponding rows in the database. When the object is stored to the database, it is assigned a new object identifier. How the new object identifier is created depends on the backend ie. the database used. It is either generated using a sequence or an auto-incrementing column. The object identifiers are unique within the class hierarchy in a single database. 

Summary:
 * one sequence per one class hierarchy (for object identifiers)
 * when the object is stored for the first time, it is given a new object identifier 

# Data Serialization

LiteSQL operates database using SQL. That implies that when storing an object, its data attributes have to be serialized to strings. Also, when an object is retrieved from the database, its data attributes have to be reconstructed from the received character data. LiteSQL supports few basic types for atomic data (which is a subject to change). Each of them has a specific column type in SQL and conversion routines for mapping C++ attributes to strings and back. 

Summary:
 * data attributes are converted to/from strings 

# Relations Between Objects

LiteSQL models all relations between objects (has-a, is-associated-with, belongs-to, etc.) using a separate table. In LiteSQL, a relation is a subset of a cartesian product of the instances of a certain set of classes and the value domains of a possibly empty set of relation attributes, that is, a relation may link one or more classes and zero or more data attributes. 

In C++, relations like these are be modelled using pointers (and high-level containers using pointers). LiteSQL stores tuples of object identifiers possibly with some data attributes to model relations. 

Summary:
 * one table per one relation (including one-to-many relations)
 * one column per one class in a relation (they refer to object identifiers)
 * one column per one data attribute in a relation
 

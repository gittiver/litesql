LiteSQL - C++ Object-Relational Persistence Framework
===========================================================================
[![Build and test (cmake based build)](https://github.com/gittiver/litesql/actions/workflows/cmake.yaml/badge.svg)](https://github.com/gittiver/litesql/actions/workflows/cmake.yaml)
[![Documentation](https://github.com/gittiver/litesql/actions/workflows/doxygen-gh-pages.yml/badge.svg)](https://github.com/gittiver/litesql/actions/workflows/doxygen-gh-pages.yml)
![travis CI](https://travis-ci.org/gittiver/litesql.svg?branch=master)
[![Appveyour Build status](https://ci.appveyor.com/api/projects/status/8np80uth6p3sul40?svg=true)](https://ci.appveyor.com/project/gittiver/litesql)

LiteSQL is a C++ library that integrates C++ objects tightly to relational database and thus provides an object persistence layer. LiteSQL supports SQLite3, PostgreSQL and MySQL as backends. LiteSQL creates tables, indexes and sequences to database and upgrades schema when needed. In addition to object persistence, LiteSQL provides object relations which can be used to model basic OO building blocks (aggregation, composition, association). Objects can be selected, filtered and ordered using template- and class-based API with type checking at compile time. 

# Features

 * SQLite3, PostgreSQL, MySQL and Oracle-backend support
 * C++ object persistence (store, update, retrieve) 
 * relational operations (filtering, ordering, referencing other objects)
 * automatic database structure maintenance (creates, updates and drops tables/indices behind the scenes)
 * C++ template based database API -> no SQL queries by hand

# License

This library is distributed under the terms of [BSD-License](@ref bsd_license)

# Status
0.3.x version is a proof-of-concept implementation, works for non-critical use

# Download
Please continue to the [Download page](http://sourceforge.net/project/showfiles.php?group_id=113304).


# Mailing List

Discussion about the library (help requests, new ideas, etc.) is very welcome at [litesql-users mailing list](http://lists.sourceforge.net/lists/listinfo/litesql-users)

# Contributions Welcome
There is a lot of interesting tasks available (the list of [active tickets](https://sourceforge.net/p/litesql/_list/tickets?source=navbar)).
Please acknowledge the license of the project before contributing. Only contributions that can be released under the license will be included.

# Documentation
 * [Quick Start](docs/quickstart.md) -- a simple complete example
 * [Contributors](docs/contributors.md) -- the persons who have dedicated their precious time for a good purpose
 * [Examples](docs/examples.md) -- code snippets for doing simple things
 * [Defining Database](docs/defining_database.md) -- how to write an XML database definition file
 * [OR-mapping model](docs/mapping_objects_to_database.md) -- how the objects are stored in the database
 

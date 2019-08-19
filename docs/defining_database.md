Writing XML Database Definition
====================================================

Before the library can be used, an XML file has to be written. The XML file defines types, objects, fields, methods and relations between objects of the database.

For those of you familiar with DTD, the file format is defined in [attachment:litesql.dtd litesql.dtd]. To make valid XML, the definition file should begin with following lines:

~~~~~~~~~~~~~~~{.xml}
<?xml version="1.0"?>
<!DOCTYPE database SYSTEM "litesql.dtd">
~~~~~~~~~~~~~~~

# Database-tag #


A database definition file has a root tag which encloses all definitions inside it. It must have exactly one 'database'-tag.

~~~~~~~~~~~~~~~{.xml}
<database name="TestDatabase" namespace="test" [include="extra.hpp"]>
   ... definitions ...
</database>
~~~~~~~~~~~~~~~

Note: attributes in [square brackets] are optional.

* 'namespace' - attribute defines C++ - namespace into which all generated classes will be put.
 * 'include' - attribute defines C++ - header file to be included in generated header file

The definition above will be extracted to C++ - class test::TestDatabase.

## Include-tag #
A database definition can have zero or more 'include'-tags. 
The model in the included file is read, objects and relations are inserted 
into the actual model. 

 * name  - is the name of the file (absolute or relative to actual working directory in the os-specific notation)
~~~~~~~~~~~~~~~{.xml}
<include name="othermodel.xml" />
~~~~~~~~~~~~~~~

# Object-tag ##

Each 'object'-tag will be extracted to Persistent-class. 'object'-tags are placed inside 'database'-tag.
~~~~~~~~~~~~~~~{.xml}
<object name="MyObject" [inherits="ParentObject"]>
   ... definitions ...
</object>
}}}
~~~~~~~~~~~~~~~

* 'inherits' - attribute defines the class from which the new class is inherited.

# Field-tag #
'field'-tags are used to define Persistent object's fields (data members) and relation attributes. They can be placed inside 'object'-tag or 'relation'-tag.

~~~~~~~~~~~~~~~{.xml}
<field name="myfield" type="integer" [default="1"] [indexed="true"]/>
<field name="color" type="integer">
    <value name="Red" value="0"/>
    <value name="Green" value="1"/>
</field>
~~~~~~~~~~~~~~~

 * possible values for 'type'-field are "integer", "bigint", "string", "boolean", "date", "time", "datetime", "float", "double" and "blob"
 * 'default' - attribute defines the initial value of field when object is created.
 * 'indexed' - attribute defines whether to CREATE INDEX for the field or not, default value is 'false'.

Enumerated values can be specified by putting 'value'-tags inside 'field'-tag.

# Method-tag #
Persistent object's methods are defined with 'method'-tag. It is placed inside 'object'-tag.
~~~~~~~~~~~~~~~{.xml}
<method name="myMethod" [returntype="void"]>
    <param name="magic" type="int"/>
</method>
~~~~~~~~~~~~~~~

 * 'returntype' - attribute defines C++ - return type for method
 * 'param' - tags are optional. They can be used to define parameters for method.

# Relation-tag #
'relation'-tag defines a relation between two or more objects.
~~~~~~~~~~~~~~~{.xml}
<relation [name="SomeRelation"] [id="1"] [unidir="true"]>
    <relate object="Person" [unique="false"] [limit="one"] [handle="cars"]/>
    <relate object="Car" [limit="many"] [handle="owner"]/>
    ... more related objects ...
    [<field name="order" type="integer"/>]
    ... more relation attributes ...
</relation>
~~~~~~~~~~~~~~~

* 'name'-attribute defines the name of C++ class of the relation
 * 'id'-attribute is used to separate multiple relations with same objects (it is appended to the name of relation table)
 * 'unidir'-attribute is used to indicate that the relation is not bidirectional, default is 'false'. Note: this has an effect only when two classes of the same type are in relation.
 * 'relate' - tag defines object in relation
 * 'limit' - attribute defines constraints for relation. Note: this has an effect only in 2-ended relations
 * 'unique' - attribute defines constraints for relation, too. Note: this cannot be used when 'limit' is specified
 * 'handle' - attribute defines identifier for relation handle, a short cut when accessing relation from a single object.
 * relation attributes can be defined using 'field'-tag
 * Note: Put relation attributes with default values last so link-method can be properly generated 

# Index-tag #
'index'-tag defines an index built on two or more fields. It can be used inside 'object'- and 'relation'-tags.
~~~~~~~~~~~~~~~{.xml}
<field name="myfield" type="integer" [default="value"] [indexed="true"]/>
<field name="color" type="integer">
    <value name="Red" value="0"/>
    <value name="Green" value="1"/>
</field>
<index [unique="true"]>
    <indexfield name="myfield"/>
    <indexfield name="color"/>
</index>
~~~~~~~~~~~~~~~

 * 'unique' - attribute defines whether there can be multiple equal field tuples

# Indexfield-tag #

'indexfield'-tag defines an index field inside multi field index. It can be used inside 'index'-tag.

# Naming Identifiers #
C++'s reserved words cannot be used as identifier names. Also, "id"- and "type"-fields are defined for each Persistent-object.


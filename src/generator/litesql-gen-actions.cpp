/* XML application for litesql.dtd.
 * Includes actions from litesql-gen.xml.
 * Generated 2006/07/19 22:47:47.
 *
 * This program was generated with the FleXML XML processor generator.
 * FleXML is Copyright � 1999-2005 Kristoffer Rose.  All rights reserved.
 * (Id: flexml.pl,v 1.52 2006/07/13 19:06:42 mquinson Exp).
 * 
 * There are two, intertwined parts to this program, part A and part B.
 *
 * Part A
 * ------
 * 
 * Some parts, here collectively called "Part A", are found in the 
 * FleXML package.  They are Copyright � 1999-2005 Kristoffer Rose. 
 * All rights reserved.
 *
 * You can redistribute, use, perform, display and/or modify "Part A"
 * provided the following two conditions hold:
 *
 * 1. The program is distributed WITHOUT ANY WARRANTY from the author of
 *    FleXML; without even the implied warranty of MERCHANTABILITY or
 *    FITNESS FOR A PARTICULAR PURPOSE.
 *
 * 2. The program distribution conditions do not in any way affect the
 *    distribution conditions of the FleXML system used to generate this
 *    file or any version of FleXML derived from that system.
 *
 * Notice that these are explicit rights granted to you for files
 * generated by the FleXML system.  For your rights in connection with
 * the FleXML system itself please consult the GNU General Public License.
 * 
 * Part B
 * ------
 * 
 * The other parts, here collectively called "Part B", and which came 
 * from the DTD used by FleXML to generate this program, can be 
 * distributed (or not, as the case may be) under the terms of whoever
 * wrote them, provided these terms respect and obey the two conditions 
 * above under the heading "Part A".
 *
 * The author of and contributors to FleXML specifically disclaim
 * any copyright interest in "Part B", unless "Part B" was written 
 * by the author of or contributors to FleXML.
 * 
 */

#include "litesql-gen.hpp"


#line 3 "litesql-gen.xml"

#include "xmlobjects.hpp"
#include "litesql-gen-main.hpp"
#include "litesql-gen.hpp"
#include "scanner.hpp"
#include <list>
using namespace std;
using namespace xml;
extern FILE* yyin;
list<Position> posStack;
string currentFile;
Database* db;
Object * obj;
Relation * rel;
Field * fld;
Method * mtd;
Index * idx;
Option* option;
IfBackend* ifBackend;
Relate* relate;

Position getPosition() {
    return Position(currentFile, yylineno);
}


void STag_database(void)
{
#line 29 "litesql-gen.xml"

db = new Database(getPosition());
db->name = safe(A_database_name);
db->include = safe(A_database_include);
db->nspace = safe(A_database_namespace);

} /* STag_database */

void STag_object(void)
{
#line 35 "litesql-gen.xml"

db->objects.push_back(obj = new Object(getPosition(), A_object_name, safe(A_object_inherits), A_object_temporary));

} /* STag_object */

void ETag_object(void)
{
#line 38 "litesql-gen.xml"

obj = NULL;

} /* ETag_object */

void STag_relation(void)
{
#line 41 "litesql-gen.xml"

db->relations.push_back(rel = new Relation(getPosition(), safe(A_relation_id), safe(A_relation_name),A_relation_unidir));

} /* STag_relation */

void ETag_relation(void)
{
#line 44 "litesql-gen.xml"

rel = NULL;

} /* ETag_relation */

void STag_option(void)
{
#line 47 "litesql-gen.xml"

option = new Option(getPosition(), A_option_name, A_option_value);
if (ifBackend)
    ifBackend->options.push_back(option);
else if (obj)
    obj->options.push_back(option);
else if (rel)
    rel->options.push_back(option);
else
    db->options.push_back(option);    

} /* STag_option */

void STag_if_d_backend(void)
{
#line 58 "litesql-gen.xml"

ifBackend = new IfBackend(getPosition(), A_if_d_backend_name);
if (obj)
    obj->ifBackends.push_back(ifBackend);
else if (rel)
    rel->ifBackends.push_back(ifBackend);
else
    db->ifBackends.push_back(ifBackend);


} /* STag_if-backend */

void ETag_if_d_backend(void)
{
#line 68 "litesql-gen.xml"

ifBackend = NULL;

} /* ETag_if-backend */

void STag_include(void)
{
#line 71 "litesql-gen.xml"

yyin = fopen(A_include_file, "r");
if (!yyin) {
    fprintf(stderr, "Could not open file %s", A_include_file);
    exit(-1);
}
posStack.push_front(getPosition());
currentFile = A_include_file;
yylineno = 1;
yypush_buffer_state(yy_create_buffer(yyin, 16834));


} /* STag_include */

void STag_field(void)
{
#line 83 "litesql-gen.xml"

if (obj) {
    obj->fields.push_back(fld =new Field(getPosition(), A_field_name, 
                                     A_field_type,safe(A_field_default),A_field_indexed,A_field_unique));
} else if (rel) {
    rel->fields.push_back(fld = new Field(getPosition(), A_field_name, 
                         A_field_type,safe(A_field_default),A_field_indexed, A_field_unique));
}

} /* STag_field */

void STag_index(void)
{
#line 92 "litesql-gen.xml"

idx = new Index(getPosition(), A_index_unique);
if (obj) 
    obj->indices.push_back(idx);
else if (rel) 
    rel->indices.push_back(idx);

} /* STag_index */

void ETag_index(void)
{
#line 99 "litesql-gen.xml"

idx = NULL;

} /* ETag_index */

void STag_indexfield(void)
{
#line 102 "litesql-gen.xml"

if (idx)
    idx->fields.push_back(IndexField(getPosition(), A_indexfield_name));

} /* STag_indexfield */

void STag_value(void)
{
#line 106 "litesql-gen.xml"

if (fld) 
    fld->value(Value(getPosition(), A_value_name, A_value_value));

} /* STag_value */

void ETag_field(void)
{
#line 110 "litesql-gen.xml"

fld = NULL;

} /* ETag_field */

void STag_method(void)
{
#line 113 "litesql-gen.xml"

if (obj) {
    obj->methods.push_back(mtd = new Method(getPosition(), A_method_name, safe(A_method_returntype)));
}

} /* STag_method */

void ETag_method(void)
{
#line 118 "litesql-gen.xml"

mtd = NULL;

} /* ETag_method */

void STag_param(void)
{
#line 121 "litesql-gen.xml"

if (mtd) 
    mtd->param(Param(getPosition(), A_param_name, A_param_type));

} /* STag_param */

void STag_relate(void)
{
#line 125 "litesql-gen.xml"

relate = new Relate(getPosition(), A_relate_object, A_relate_limit, A_relate_unique, safe(A_relate_handle), safe(A_relate_remotehandle));
if (rel)
    rel->related.push_back(relate);
else if (obj)
    obj->related.push_back(relate);

} /* STag_relate */

void ETag_database(void)
{
#line 132 "litesql-gen.xml"

    generateCode(*db);    

} /* ETag_database */


#line 140 "litesql-gen.xml"

extern FILE * yyin;
int main(int argc, char **argv) {
    return litesql_gen_main(argc, argv, &yyin);
}


/* XML application entry points. */
void STag_type(void) {}
void ETag_type(void) {}
void ETag_include(void) {}
void ETag_option(void) {}
void ETag_indexfield(void) {}
void ETag_value(void) {}
void ETag_param(void) {}
void ETag_relate(void) {}

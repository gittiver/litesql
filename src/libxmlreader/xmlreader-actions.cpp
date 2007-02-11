#include "flexml-header.hpp"
#include "xmltypes.hpp"
#include "scanner.hpp"
#include <list>

using namespace std;
using namespace xml;
extern FILE* yyin;
list<Position> posStack;
string currentFile;
Database* xmlReaderDb;
static Object* obj;
static Relation* rel;
static Interface* iface;
static Field* fld;
static Type* type;
static Method* mtd;
static Index* idx;
static Option* option;
static Relate* relate;

Position getPosition() {

    return Position(currentFile, yylineno);
}


void STag_database(void) {

    xmlReaderDb = new Database(getPosition(),
                               safe(A_database_name),
                               safe(A_database_include),
                               safe(A_database_namespace));
} 

void ETag_database(void) {}

void STag_object(void) {
    obj = new Object(getPosition(), 
                     A_object_name, 
                     safe(A_object_inherits), 
                     A_object_temporary);


    xmlReaderDb->objects.push_back(obj);
}

void ETag_object(void) {

    obj = NULL;
}

void STag_relation(void) {
    rel = new Relation(getPosition(), 
                       safe(A_relation_id), 
                       safe(A_relation_name),
                       A_relation_unidir);

    xmlReaderDb->relations.push_back(rel);
}

void ETag_relation(void) {

    rel = NULL;
}

void STag_option(void) {

    option = new Option(getPosition(), 
                        A_option_name, 
                        A_option_value,
                        A_option_backend);

    if (obj)
        obj->options.push_back(option);
    else if (rel)
        rel->options.push_back(option);
    else
        xmlReaderDb->options.push_back(option);    

} 

void ETag_option(void) {}

void STag_include(void) {

    yyin = fopen(A_include_file, "r");
    if (!yyin) {
        fprintf(stderr, "Could not open file %s", A_include_file);
        exit(-1);
    }
    posStack.push_front(getPosition());
    currentFile = A_include_file;
    yylineno = 1;
    yypush_buffer_state(yy_create_buffer(yyin, 16834));
} 

void ETag_include(void) {}


void STag_type(void) {

    type = new Type(getPosition(), 
                    A_type_name);
    xmlReaderDb->types.push_back(type);
}

void ETag_type(void) {


    type = NULL;
}

void STag_field(void) {


    if (obj) {
        obj->fields.push_back(fld =new Field(getPosition(), A_field_name, 
                    A_field_type,safe(A_field_default),A_field_indexed,A_field_unique));
    } else if (rel) {
        rel->fields.push_back(fld = new Field(getPosition(), A_field_name, 
                    A_field_type,safe(A_field_default),A_field_indexed, A_field_unique));
    }

} 

void STag_index(void) {


    idx = new Index(getPosition(), A_index_unique);
    if (obj) 
        obj->indices.push_back(idx);
    else if (rel) 
        rel->indices.push_back(idx);
}

void ETag_index(void) {


    idx = NULL;
}

void STag_indexfield(void) {


    if (idx)
        idx->fields.push_back(new IndexField(getPosition(), A_indexfield_name));

}

void ETag_indexfield(void) {}

void STag_value(void) {


    if (fld) 
        fld->values.push_back(new Value(getPosition(), A_value_name, A_value_value));
    else if (type)
        type->values.push_back(new Value(getPosition(), A_value_name, A_value_value));

}

void ETag_value(void) {}

void ETag_field(void) {


    fld = NULL;

} 

void STag_method(void) {

    mtd = new Method(getPosition(), A_method_name, safe(A_method_returntype), A_method_const);
    if (obj) 
        obj->methods.push_back(mtd);
    else if (iface) 
        iface->methods.push_back(mtd);
}

void ETag_method(void) {


    mtd = NULL;
}

void STag_param(void) {

    if (mtd) 
        mtd->params.push_back(new Param(getPosition(), 
                                        A_param_name, 
                                        A_param_type));

}

void ETag_param(void) {}

void STag_relate(void) {

    relate = new Relate(getPosition(), 
                        A_relate_object, 
                        A_relate_interface,
                        A_relate_limit, 
                        A_relate_unique, 
                        safe(A_relate_handle));
    if (rel)
        rel->related.push_back(relate);
}

void ETag_relate(void) {}



void STag_check(void) {
    if (obj)
        obj->checks.push_back(new Check(getPosition(), 
                                        A_check_function,
                                        A_check_param,
                                        A_check_oncreate,
                                        A_check_ondelete,
                                        A_check_onupdate,
                                        A_check_onlink,
                                        A_check_onunlink));
}

void ETag_check(void) {}

void STag_interface(void) {

    iface = new Interface(getPosition(), A_interface_name);
    xmlReaderDb->interfaces.push_back(iface);
}

void ETag_interface(void) {}

void STag_implements(void) {

    if (obj)
        obj->implements.push_back(new Implements(getPosition(),
                                                 A_implements_interface));
}

void ETag_implements(void) {}

void STag_represent(void) {
    
    type->represents.push_back(new Represent(getPosition(),
                                             A_represent_as,
                                             safe(A_represent_target)));                                             
}

void ETag_represent(void) {}

void STag_store(void) {
    
    type->stores.push_back(new Store(getPosition(),
                                     A_store_as,
                                     safe(A_store_backend)));
}

void ETag_store(void) {}



#include "xmlreader-actions.hpp"
#include "xmltypes.hpp"

#include "scanner.hpp"
#include <list>

using namespace std;
using namespace xml;
extern FILE* yyin;
list<Position> posStack;
string currentFile;
Database* xmlReaderDb;
static Object * obj;
static Relation * rel;
static Field * fld;
static Type* type;
static Method * mtd;
static Index * idx;
static Option* option;
static IfBackend* ifBackend;
static IfTarget* ifTarget;
static Relate* relate;

Position getPosition() {

    return Position(currentFile, yylineno);
}


void STag_database(void) {


    xmlReaderDb = new Database(getPosition());
    xmlReaderDb->name = safe(A_database_name);
    xmlReaderDb->include = safe(A_database_include);
    xmlReaderDb->nspace = safe(A_database_namespace);
} 

void STag_object(void) {


    xmlReaderDb->objects.push_back(obj = new Object(getPosition(), A_object_name, safe(A_object_inherits), A_object_temporary));

}

void ETag_object(void) {


    obj = NULL;
}

void STag_relation(void) {


    xmlReaderDb->relations.push_back(rel = new Relation(getPosition(), safe(A_relation_id), safe(A_relation_name),A_relation_unidir));
}

void ETag_relation(void) {


    rel = NULL;
}

void STag_option(void) {


    option = new Option(getPosition(), A_option_name, A_option_value);
    if (ifBackend)
        ifBackend->options.push_back(option);
    else if (ifTarget)
        ifTarget->options.push_back(option);
    else if (obj)
        obj->options.push_back(option);
    else if (rel)
        rel->options.push_back(option);
    else
        xmlReaderDb->options.push_back(option);    

} 

void STag_if_d_backend(void) {


    ifBackend = new IfBackend(getPosition(), A_if_d_backend_name);
    if (obj)
        obj->ifBackends.push_back(ifBackend);
    else if (rel)
        rel->ifBackends.push_back(ifBackend);
    else
        xmlReaderDb->ifBackends.push_back(ifBackend);


}

void ETag_if_d_backend(void) {


    ifBackend = NULL;
}

void STag_if_d_target(void) {


    ifTarget = new IfTarget(getPosition(), A_if_d_target_name);
    xmlReaderDb->ifTargets.push_back(ifTarget);
}

void ETag_if_d_target(void) {


    ifTarget = NULL;
} 

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

void STag_type(void) {


    type = new Type(getPosition(), A_type_name, A_type_class, A_type_sqltype);
    if (ifBackend)
        ifBackend->types.push_back(type);
    else if (ifTarget)
        ifTarget->types.push_back(type);
    else
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
        idx->fields.push_back(IndexField(getPosition(), A_indexfield_name));

}

void STag_value(void) {


    if (fld) 
        fld->values.push_back(Value(getPosition(), A_value_name, A_value_value));
    else if (type)
        type->values.push_back(Value(getPosition(), A_value_name, A_value_value));

}

void ETag_field(void) {


    fld = NULL;

} 

void STag_method(void) {


    if (obj) {
        obj->methods.push_back(mtd = new Method(getPosition(), A_method_name, safe(A_method_returntype), A_method_const));
    }
}

void ETag_method(void) {


    mtd = NULL;
}

void STag_param(void)
{


if (mtd) 
    mtd->params.push_back(Param(getPosition(), A_param_name, A_param_type));

}

void STag_relate(void) {


    relate = new Relate(getPosition(), A_relate_object, A_relate_limit, A_relate_unique, safe(A_relate_handle), safe(A_relate_remotehandle));
    if (rel)
        rel->related.push_back(relate);
    else if (obj)
        obj->related.push_back(relate);
}

void ETag_database(void) {


} 


extern FILE * yyin;


/* XML application entry points. */
void ETag_include(void) {}
void ETag_option(void) {}
void ETag_indexfield(void) {}
void ETag_value(void) {}
void ETag_param(void) {}
void ETag_relate(void) {}

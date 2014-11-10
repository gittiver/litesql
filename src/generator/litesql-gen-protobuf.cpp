#include "litesql-gen-protobuf.hpp"
#include <fstream>
#include <string.h>

using namespace std;
using namespace xml;

const char* ProtoBufClassGenerator::NAME = "protobuf";

string toProtoBufType(AT_field_type field_type)
{
    switch (field_type)
    {
        case A_field_type_integer:
            return "int32";
        case A_field_type_float:
            return "float";
        case A_field_type_double:
            return "double";
        case A_field_type_boolean:
            return "bool";
        case A_field_type_date:
            return "string";
        case A_field_type_time:
            return "string";
        case A_field_type_datetime:
            return "string";
        case A_field_type_blob:
            return "bytes";
        case A_field_type_string:
            return "string";
        default:
            return "unknown";
    }
}

void generateMessage(const ObjectModel* model, ostream& os)
{
    os << "package ss.db;" << endl << endl;

    string prefix("");
    string indent("    ");

    for (ObjectSequence::const_iterator it = model->objects.begin();
            it != model->objects.end(); it++)
    {
        os << "message " << prefix << (*it)->name << endl << "{" << endl;
        int sequence = 1;

        for (Field::sequence::const_iterator fit = (*it)->fields.begin();
                fit != (*it)->fields.end(); fit++)
        {
            if (strcmp((*fit)->name.c_str(), "id") != 0
                    && strcmp((*fit)->name.c_str(), "type") != 0)
            {
                os << indent << "optional "
                        << toProtoBufType((*fit)->type) << ' '
                        << (*fit)->name << " = " << sequence++ << ";" << endl;
            }
        }

        os << "}" << endl << endl;
    }
}

bool ProtoBufClassGenerator::generateCode(const ObjectModel* model)
{
    ofstream os(getOutputFilename(model->db->name + ".proto").c_str());
    generateMessage(model, os);
    os.close();
    return true;
}


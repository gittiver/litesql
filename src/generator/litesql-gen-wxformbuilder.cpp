#include "litesql-gen-wxformbuilder.hpp"

#include <fstream>

using namespace std;
using namespace xml;
using namespace litesql;

const char* wxFormBuilderGenerator::NAME = "wxFormBuilder";

wxFormBuilderGenerator::wxFormBuilderGenerator() 
    : CodeGenerator(NAME) 
{ }

void wxFormBuilderGenerator::setOutputFilename(const std::string& filename)
{
    m_outputFilename=filename;
}

static bool generateFieldUI(const xml::Field::Ptr& field,ostream& os,size_t indent)
{
    os  << "                <object class=\"sizeritem\" expanded=\"1\">" << endl
        << "                    <property name=\"border\">5</property>" << endl
        << "                    <property name=\"flag\">wxALL|wxEXPAND</property>" << endl
        << "                    <property name=\"proportion\">1</property>" << endl
        << "                    <object class=\"wxStaticText\" expanded=\"1\">" << endl
        << "                        <property name=\"label\">" << capitalize(field->name)    << "</property>" << endl
        << "                        <property name=\"name\">lbl_" << capitalize(field->name) << "</property>" << endl
        << "                    </object>" << endl
        << "                </object>" << endl
        << "                <object class=\"sizeritem\" expanded=\"1\">" << endl
        << "                    <property name=\"border\">5</property>" << endl
        << "                    <property name=\"flag\">wxALL|wxFIXED_MINSIZE|wxEXPAND</property>" << endl
        << "                    <property name=\"proportion\">2</property>" << endl;
        switch(field->type)
        {
            case A_field_type_boolean:
                os << "                    <object class=\"wxCheckBox\" expanded=\"1\">" << endl;
                break;
            case A_field_type_integer:
                os << "                    <object class=\"wxSpinCtrl\" expanded=\"1\">" << endl
                   << "                        <property name=\"name\">m_spin_" << capitalize(field->name) << "</property>" << endl;
          break;

            case A_field_type_date:
                os << "                    <object class=\"wxDatePickerCtrl\" expanded=\"1\">" << endl
                   << "                        <property name=\"name\">m_datePicker_" << capitalize(field->name) << "</property>" << endl;
        
                break;

            case A_field_type_datetime:
            case A_field_type_time:
                
            case A_field_type_string:
            default:
                os << "                    <object class=\"wxTextCtrl\" expanded=\"1\">"  << endl
                   << "                        <property name=\"maxlength\">0</property>" << endl
                   << "                        <property name=\"name\">m_textCtrl_" << capitalize(field->name) << "</property>" << endl;
        }
        
        os  << "                    </object>" << endl
            << "                </object>" << endl;
    return true;
}


static bool generateDetailPanel(const xml::ObjectPtr& object,ostream& os,size_t indent)
{
    os << "      <object class=\"Panel\" expanded=\"0\">" << endl
        << "            <property name=\"minimum_size\">280,80</property>" << endl
        << "            <property name=\"name\">" << object->name << "Panel</property>" << endl
        << "            <property name=\"pos\" />" << endl
        << "            <property name=\"size\">280," << object->fields.size() * 20 << "</property>" << endl
        << "            <object class=\"wxFlexGridSizer\" expanded=\"1\">" << endl
        << "                <property name=\"cols\">2</property>" << endl
        << "                <property name=\"flexible_direction\">wxBOTH</property>" << endl
        << "                <property name=\"growablecols\">1</property>" << endl
        << "                <property name=\"growablerows\" />" << endl
        << "                <property name=\"hgap\">0</property>" << endl
        << "                <property name=\"minimum_size\" />" << endl
        << "                <property name=\"name\">gFieldSizer</property>" << endl
        << "                <property name=\"non_flexible_grow_mode\">wxFLEX_GROWMODE_SPECIFIED</property>" << endl
        << "                <property name=\"permission\">none</property>" << endl
        << "                <property name=\"rows\">" << object->fields.size()<< "</property>" << endl
        << "                <property name=\"vgap\">0</property>" << endl;

    for (Field::sequence::const_iterator field_it = object->fields.begin();
        field_it != object->fields.end();
        field_it++)
    {
        if (((*field_it)->name!="id") && ((*field_it)->name!="type"))
        {
            generateFieldUI(*field_it,os,indent);
        }
    }


    os  << "           </object>" << endl
        << "        </object>" << endl;

    return true;
}

bool wxFormBuilderGenerator::generate(const xml::ObjectPtr& object,ostream& os,size_t indent)
{
    generateDetailPanel(object,os,indent);
//    generateListPanel(object,os,indent);

    return true;
}


bool wxFormBuilderGenerator::generate(const Relation::Ptr &  relation,ostream& os,size_t indent)
{
    return true;
}

bool wxFormBuilderGenerator::generateCode(const ObjectModel* model)
{
    bool success;
    string ofname =   m_outputFilename.empty() 
        ?  model->db->name +".fbp"
        :  m_outputFilename;

    ofstream ofs(getOutputFilename(ofname).c_str());

    ofs << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>" << endl
        << "<wxFormBuilder_Project>" << endl
        << "    <FileVersion major=\"1\" minor=\"10\" />" << endl
        << "    <object class=\"Project\" expanded=\"1\">" << endl
        << "        <property name=\"class_decoration\" />" << endl
        << "        <property name=\"code_generation\">C++</property>" << endl
        << "        <property name=\"disconnect_events\">1</property>" << endl
        << "        <property name=\"encoding\">UTF-8</property>" << endl
        << "        <property name=\"event_generation\">connect</property>" << endl
        << "        <property name=\"file\">ui</property>" << endl
        << "        <property name=\"first_id\">1000</property>" << endl
        << "        <property name=\"help_provider\">wxHelpControllerHelpProvider</property>" << endl
        << "        <property name=\"internationalize\">1</property>" << endl
        << "        <property name=\"name\">Litesql</property>" << endl
        << "        <property name=\"namespace\">&quot;ui&quot;</property>" << endl
        << "        <property name=\"path\">../</property>" << endl
        << "        <property name=\"precompiled_header\" />" << endl 
        << "        <property name=\"relative_path\">1</property>" << endl
        << "        <property name=\"use_enum\">1</property>" << endl
        << "        <property name=\"use_microsoft_bom\">0</property>" << endl;

    success = CodeGenerator::generate(model->objects,ofs,2);
    success &= CodeGenerator::generate(model->relations,ofs,2);

    ofs << "    </object>" << endl
        << "</wxFormBuilder_Project>" << endl;
    ofs.close();
    return success;
}


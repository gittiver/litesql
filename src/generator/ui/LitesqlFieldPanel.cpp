#include "LitesqlFieldPanel.h"
#include "objectmodel.hpp"

#include "ddx.h"

using namespace xml;

IMPLEMENT_DYNAMIC_CLASS(LitesqlFieldPanel,ui::FieldPanel)

/////////////////////////////////////////////////////////////////////////////

LitesqlFieldPanel::LitesqlFieldPanel( wxWindow* parent , Field* pField)
:
FieldPanel( parent ),
m_pField(pField)
{
  m_textCtrlName->Enable(m_pField->isEditable());
  m_choiceInheritsFrom->Enable(m_pField->isEditable());
  m_textCtrlDefaultValue->Enable(m_pField->isEditable()); 
  m_checkBoxIndexed->Enable(m_pField->isEditable());
  m_checkBoxUnique->Enable(m_pField->isEditable());

  
  m_textCtrlName->SetValidator(StdStringValidator(wxFILTER_ALPHANUMERIC,&m_pField->name));
  m_textCtrlDefaultValue->SetValidator(StdStringValidator(wxFILTER_ALPHANUMERIC,&m_pField->default_));
  
  m_choiceInheritsFrom->Append(FieldTypeValidator::FIELDTYPES);
  m_choiceInheritsFrom->SetValidator(FieldTypeValidator(m_pField));

  m_checkBoxIndexed->SetValidator(FieldIndexedValidator(m_pField));
  m_checkBoxUnique->SetValidator(FieldUniqueValidator(m_pField));
}

// FieldTypeValidator implementation
/****************************************************************************/
const wxString FTSTRING[] = { 
               _(""),
               _("boolean"),
               _("integer"),
               _("string"),
               _("float"),
               _("double"),
               _("time"),
               _("date"),
               _("datetime"),
               _("blob") 
};  

const wxArrayString FieldTypeValidator::FIELDTYPES(sizeof(FTSTRING)/sizeof(FTSTRING[0]),FTSTRING);

IMPLEMENT_DYNAMIC_CLASS(FieldTypeValidator,wxGenericValidator)

FieldTypeValidator::FieldTypeValidator (xml::Field *val)
: m_pField(val),
  wxGenericValidator(&value)
{
}

FieldTypeValidator::FieldTypeValidator (const FieldTypeValidator& val)
: wxGenericValidator(val)
{
  m_pField= val.m_pField;
  value = val.value;
  Copy(val);
  m_pString = &value;
}

wxObject *FieldTypeValidator::Clone() const
{ return new FieldTypeValidator(*this); }
    
  // Called to transfer data to the window
bool FieldTypeValidator::TransferToWindow() 
{
  value = wxString::FromUTF8(litesql::toString(m_pField->type));
  return wxGenericValidator::TransferToWindow();
}

    // Called to transfer data from the window
bool FieldTypeValidator::TransferFromWindow()
{
  bool rval = wxGenericValidator::TransferFromWindow();
  m_pField->type = litesql::field_type(value.ToUTF8());
  return rval;
}

// FieldIndexedValidator implementation
/****************************************************************************/

IMPLEMENT_DYNAMIC_CLASS(FieldIndexedValidator,wxGenericValidator)

FieldIndexedValidator::FieldIndexedValidator (xml::Field* pField)
: m_pField(pField),
  wxGenericValidator(&isIndexed)
{
}

FieldIndexedValidator::FieldIndexedValidator (const FieldIndexedValidator& val)
: wxGenericValidator(val)
{
  m_pField= val.m_pField;
  Copy(val);
  m_pBool = &isIndexed;
}

wxObject *FieldIndexedValidator::Clone() const
{ return new FieldIndexedValidator(*this); }
    
  // Called to transfer data to the window
bool FieldIndexedValidator::TransferToWindow() 
{
  isIndexed = m_pField->isIndexed();
  return wxGenericValidator::TransferToWindow();
}

    // Called to transfer data from the window
bool FieldIndexedValidator::TransferFromWindow()
{
  bool rval = wxGenericValidator::TransferFromWindow();
  if (rval) 
  {
    m_pField->indexed = isIndexed ? A_field_indexed_true : A_field_indexed_false; 
  }
  return rval;
}

// FieldUniqueValidator implementation
/****************************************************************************/

IMPLEMENT_DYNAMIC_CLASS(FieldUniqueValidator,wxGenericValidator)

FieldUniqueValidator::FieldUniqueValidator (xml::Field* pField)
: m_pField(pField),
  wxGenericValidator(&isUnique)
{
}

FieldUniqueValidator::FieldUniqueValidator (const FieldUniqueValidator& val)
: wxGenericValidator(val)
{
  m_pField= val.m_pField;
  Copy(val);
  m_pBool = &isUnique;
}

wxObject *FieldUniqueValidator::Clone() const
{ return new FieldUniqueValidator(*this); }
    
  // Called to transfer data to the window
bool FieldUniqueValidator::TransferToWindow() 
{
  isUnique = m_pField->isUnique();
  return wxGenericValidator::TransferToWindow();
}

    // Called to transfer data from the window
bool FieldUniqueValidator::TransferFromWindow()
{
  bool rval = wxGenericValidator::TransferFromWindow();
  if (rval) 
  {
    m_pField->unique = isUnique ? A_field_unique_true : A_field_unique_false; 
  }
  return rval;
}



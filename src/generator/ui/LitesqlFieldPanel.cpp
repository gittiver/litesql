#include "LitesqlFieldPanel.h"
#include "objectmodel.hpp"

#include "ddx.h"

using namespace xml;

IMPLEMENT_DYNAMIC_CLASS(LitesqlFieldPanel,ui::FieldPanel)

/////////////////////////////////////////////////////////////////////////////

LitesqlFieldPanel::LitesqlFieldPanel( wxWindow* parent , Field::Ptr pField)
:
FieldPanel( parent ),
m_pField(pField)
{
  m_textCtrlName->Enable(m_pField->isEditable());
  m_choiceFieldtype->Enable(m_pField->isEditable());
  m_textCtrlDefaultValue->Enable(m_pField->isEditable()); 
  m_checkBoxIndexed->Enable(m_pField->isEditable());
  m_checkBoxUnique->Enable(m_pField->isEditable());
  m_listValues->Enable(m_pField->isEditable());
  
  m_textCtrlName->SetValidator(StdStringValidator(wxFILTER_ALPHANUMERIC,&m_pField->name));
  m_textCtrlDefaultValue->SetValidator(StdStringValidator(wxFILTER_ALPHANUMERIC,&m_pField->default_));
  // \TODO get Fieldtypes from litesql::model
  m_choiceFieldtype->Append(FieldTypeValidator::FIELDTYPES);
  m_choiceFieldtype->SetValidator(FieldTypeValidator(m_pField));

  m_checkBoxIndexed->SetValidator(FieldIndexedValidator(m_pField));
  m_checkBoxUnique->SetValidator(FieldUniqueValidator(m_pField));

  m_listValues->SetValidator(FieldValuesValidator(m_pField));
  
  m_listValues->InsertColumn(0, _("Name") , wxLIST_FORMAT_LEFT, m_listValues->GetSize().GetWidth()/2 );
  m_listValues->InsertColumn(1, _("Value"), wxLIST_FORMAT_LEFT, wxLIST_AUTOSIZE );
}

void LitesqlFieldPanel::OnAddValue( wxCommandEvent& WXUNUSED(event) )
{
  long index = m_listValues->InsertItem(m_listValues->GetItemCount(), _("newValue"));
  m_listValues->SetItem(index, 1, _("?"));
}

void LitesqlFieldPanel::OnRemoveValue( wxCommandEvent& WXUNUSED(event) )
{
  long item = -1;
  for ( ;; )
  {
    item = m_listValues->GetNextItem(item,
      wxLIST_NEXT_ALL,
      wxLIST_STATE_SELECTED);
    if ( item == -1 )
      break;

    // this item is selected - do whatever is needed with it
    m_listValues->DeleteItem(item);
  }
}

// FieldTypeValidator implementation
/****************************************************************************/
const wxString FTSTRING[] = { 
               _(""),
               _("boolean"),
               _("integer"),
               _("bigint"),
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

FieldTypeValidator::FieldTypeValidator (xml::Field::Ptr val)
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
  value = wxString::FromUTF8(toAttributeString(m_pField->type));
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

FieldIndexedValidator::FieldIndexedValidator (xml::Field::Ptr pField)
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

FieldUniqueValidator::FieldUniqueValidator (xml::Field::Ptr pField)
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


// FieldValuesValidator implementation
/****************************************************************************/
IMPLEMENT_DYNAMIC_CLASS(FieldValuesValidator,wxValidator)

FieldValuesValidator::FieldValuesValidator (Field::Ptr val)
: m_pField(val),
  wxValidator()
{
  // \TODO add values init
}

FieldValuesValidator::FieldValuesValidator (const FieldValuesValidator& val)
{
  Copy(val);
  m_pField= val.m_pField;
}

wxObject *FieldValuesValidator::Clone() const
{ return new FieldValuesValidator(*this); }
    
  // Called to transfer data to the window
bool FieldValuesValidator::TransferToWindow() 
{
  wxWindow * pWin = GetWindow();
  if ( !pWin || !pWin->IsKindOf(CLASSINFO(wxListCtrl)) )
  {
    return false;    
  }
  else {
    wxListCtrl* pList = (wxListCtrl*)pWin; 
    pList->DeleteAllItems();
    size_t pos = 0;
    for (vector<Value>::const_iterator it = m_pField->values.begin(); it != m_pField->values.end();it++)
    {
      long index = pList->InsertItem(pList->GetItemCount(),wxString::FromUTF8(it->name.c_str()),pos++);
      pList->SetItem(index,1,wxString::FromUTF8(it->value.c_str()),pos++);

    }
  }
  return true;
}

    // Called to transfer data from the window
bool FieldValuesValidator::TransferFromWindow()
{
  wxWindow * pWin = GetWindow();
  if ( !pWin || !pWin->IsKindOf(CLASSINFO(wxListCtrl)) )
  {
    return false;    
  }
  else
  {
    wxListCtrl* pList = (wxListCtrl*)pWin; 
    m_pField->values.clear();
    
    ;
  
    wxListItem info;
    for( long index = pList->GetNextItem(-1); 
         index !=wxNOT_FOUND;
         index = pList->GetNextItem(index) 
       )
    {
      
      string name(pList->GetItemText(index).ToUTF8());
      
      // get column 1 as value
      info.SetId(index);
      info.SetColumn(1);
      info.SetMask(wxLIST_MASK_TEXT);
      pList->GetItem(info);
      
      string value(info.GetText().ToUTF8());
      
      xml::Value v(name,value);
      m_pField->values.push_back( v );
    }
    
    return true;
  }
}

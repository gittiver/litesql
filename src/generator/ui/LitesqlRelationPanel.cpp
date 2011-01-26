#include "LitesqlRelationPanel.h"

#include "objectmodel.hpp"
#include "ddx.h"

using namespace xml;

/* RelateTypeValidator Implementation */
IMPLEMENT_DYNAMIC_CLASS(RelateTypeValidator,wxGenericValidator)

RelateTypeValidator::RelateTypeValidator (xml::Relate::counted_ptr val)
: m_pRelate(val),
  wxGenericValidator(&value)
{
}

RelateTypeValidator::RelateTypeValidator (const RelateTypeValidator& val)
: wxGenericValidator(val)
{
  m_pRelate = val.m_pRelate;
  value = val.value;
  Copy(val);
  m_pString = &value;
}

wxObject *RelateTypeValidator::Clone() const
{ return new RelateTypeValidator(*this); }
    
  // Called to transfer data to the window
bool RelateTypeValidator::TransferToWindow() 
{
  value = wxString::FromUTF8(m_pRelate->objectName.c_str());
  return wxGenericValidator::TransferToWindow();
}

    // Called to transfer data from the window
bool RelateTypeValidator::TransferFromWindow()
{
  bool rval = wxGenericValidator::TransferFromWindow();
  m_pRelate->objectName = value.ToUTF8();
  return rval;
}

LitesqlRelationPanel::LitesqlRelationPanel( wxWindow* parent, Relation::counted_ptr& pRelation )
: ui::RelationPanel( parent ),
  m_pRelation(pRelation)
{
  m_textCtrlName->SetValidator(StdStringValidator(wxFILTER_ALPHANUMERIC,&m_pRelation->name));
}

LitesqlRelatePanel::LitesqlRelatePanel( wxWindow* parent, Relate::counted_ptr& pRelate )
: ui::RelatePanel( parent ),
  m_pRelate(pRelate)
{
}

void LitesqlRelatePanel::setBaseClasses(const ObjectSequence& baseClasses)
{
  m_choiceRelatedObject->Clear();
  for (ObjectSequence::const_iterator it = baseClasses.begin();
      it != baseClasses.end();
      it++)
  {
    m_choiceRelatedObject->Append(wxString::FromUTF8((*it)->name.c_str()));
  }
}




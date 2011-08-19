#include "LitesqlRelationPanel.h"

#include "objectmodel.hpp"
#include "ddx.h"
#include <wx/log.h>

using namespace xml;

/* RelateTypeValidator Implementation */
IMPLEMENT_DYNAMIC_CLASS(RelateTypeValidator,wxGenericValidator)

RelateTypeValidator::RelateTypeValidator (xml::Relate::Ptr val)
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

/* RelateLimitValidator Implementation */
IMPLEMENT_DYNAMIC_CLASS(RelateLimitValidator,wxGenericValidator)

RelateLimitValidator::RelateLimitValidator (xml::Relate::Ptr val)
: m_pRelate(val),
  wxGenericValidator(&value)
{
}

RelateLimitValidator::RelateLimitValidator (const RelateLimitValidator& val)
: wxGenericValidator(val)
{
  m_pRelate = val.m_pRelate;
  value = val.value;
  Copy(val);
  m_pString = &value;
}

wxObject *RelateLimitValidator::Clone() const
{ return new RelateLimitValidator(*this); }
    
  // Called to transfer data to the window
bool RelateLimitValidator::TransferToWindow() 
{
  if (m_pRelate->unique == A_relate_unique_true)
  {
    value = _T("unique");
    if (m_pRelate->limit!=AU_relate_limit)
    {
      wxLogWarning(_T("m_pRelate->limit!=AU_relate_limit while m_pRelate->limit!=AU_relate_limit"));
    }
  }
  else { 
    switch(m_pRelate->limit)
    {
    case A_relate_limit_one: 
      value = _T("one");
      break;

    case A_relate_limit_many:
      value = _T("many");
      break;

    default:
      value = _T("");
    }
  }
  return wxGenericValidator::TransferToWindow();
}

    // Called to transfer data from the window
bool RelateLimitValidator::TransferFromWindow()
{
  bool rval = wxGenericValidator::TransferFromWindow();
  if (rval)
  {
    if (value==_T("one"))
    {
      m_pRelate->limit = A_relate_limit_one;
      m_pRelate->unique = AU_relate_unique;
    } 
    else if (value==_T("many"))
    {
      m_pRelate->limit = A_relate_limit_many;
      m_pRelate->unique = AU_relate_unique;
    }
    else if (value==_T("unique"))
    {
      m_pRelate->limit = AU_relate_limit;
      m_pRelate->unique = A_relate_unique_true;
    }
    else
    {
      wxLogWarning(_("unknown value %s"),value.c_str());
    }
  }
  return rval;
}
/*
typedef enum { AU_relate_limit, A_relate_limit_one,A_relate_limit_many } AT_relate_limit;
typedef enum { AU_relate_unique, A_relate_unique_true,A_relate_unique_false } AT_relate_unique;
*/


LitesqlRelationPanel::LitesqlRelationPanel( wxWindow* parent, Relation::Ptr& pRelation )
: ui::RelationPanel( parent ),
  m_pRelation(pRelation)
{
  m_textCtrlName->SetValidator(StdStringValidator(wxFILTER_ALPHANUMERIC,&m_pRelation->name));
}

LitesqlRelatePanel::LitesqlRelatePanel( wxWindow* parent, Relate::Ptr& pRelate )
: ui::RelatePanel( parent ),
  m_pRelate(pRelate)
{
  m_textCtrlName->SetValidator(StdStringValidator(wxFILTER_ALPHANUMERIC,&m_pRelate->handle));
  m_choiceRelatedObject->SetValidator(RelateTypeValidator(m_pRelate));
  m_choiceLimit->SetValidator(RelateLimitValidator(m_pRelate));
// TODO handle limit

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




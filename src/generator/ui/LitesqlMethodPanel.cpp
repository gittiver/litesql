#include "LitesqlMethodPanel.h"
#include "objectmodel.hpp"

using namespace xml;

LitesqlMethodPanel::LitesqlMethodPanel( wxWindow* parent , Method* pMethod)
:
MethodPanel( parent ),
m_pMethod(pMethod)
{
  // m_textCtrlName->Enable(m_pField->isEditable());
  //m_choiceInheritsFrom->Enable(m_pField->isEditable());
  //m_textCtrlDefaultValue->Enable(m_pField->isEditable()); 
  //m_checkBoxIndexed->Enable(m_pField->isEditable());
  //m_checkBoxUnique->Enable(m_pField->isEditable());

  //m_choiceInheritsFrom->Append(uiField::FIELDTYPES);

}

bool LitesqlMethodPanel::TransferData(bool toWindow)
{
  transfer_text(m_textCtrlName,m_pMethod->name,toWindow);
  //TRANSFER_CHOICE(m_choiceInheritsFrom,m_pField->type,toWindow);
  //TRANSFER_TEXT(m_textCtrlDefaultValue,m_pField->field()->default_,toWindow); 
  //TRANSFER_BOOL(m_checkBoxIndexed,m_pField->indexed,toWindow);
  //TRANSFER_BOOL(m_checkBoxUnique,m_pField->unique,toWindow);
  return true;
}

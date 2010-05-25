#include "LitesqlFieldPanel.h"
#include "objectmodel.hpp"

using namespace xml;

LitesqlFieldPanel::LitesqlFieldPanel( wxWindow* parent , uiField* pField)
:
FieldPanel( parent ),
m_pField(pField)
{
  m_textCtrlName->Enable(m_pField->isEditable());
  m_choiceInheritsFrom->Enable(m_pField->isEditable());
  m_textCtrlDefaultValue->Enable(m_pField->isEditable()); 
  m_checkBoxIndexed->Enable(m_pField->isEditable());
  m_checkBoxUnique->Enable(m_pField->isEditable());

  m_choiceInheritsFrom->Append(uiField::FIELDTYPES);

}

bool LitesqlFieldPanel::TransferData(bool toWindow)
{
  TRANSFER_TEXT(m_textCtrlName,m_pField->field()->name,toWindow);
  TRANSFER_CHOICE(m_choiceInheritsFrom,m_pField->type,toWindow);
  TRANSFER_TEXT(m_textCtrlDefaultValue,m_pField->field()->default_,toWindow); 
  TRANSFER_BOOL(m_checkBoxIndexed,m_pField->indexed,toWindow);
  TRANSFER_BOOL(m_checkBoxUnique,m_pField->unique,toWindow);
  return true;
}

#include "LitesqlObjectPanel.h"
#include "objectmodel.hpp"

#include "ddx.h"

using namespace xml;

IMPLEMENT_DYNAMIC_CLASS(LitesqlObjectPanel,ui::ObjectPanel)


LitesqlObjectPanel::LitesqlObjectPanel( wxWindow* parent, vector<Object*> baseClasses,Object* pObject  )
:
ui::ObjectPanel( parent ),
m_pObject(pObject)
{
  m_choiceInheritsFrom->Append(_T(""));
  for (vector<Object*>::const_iterator it = baseClasses.begin();
      it != baseClasses.end();
      it++)
  {
    m_choiceInheritsFrom->Append(wxString::FromUTF8((*it)->name.c_str()));
  }
}

xml::Object* LitesqlObjectPanel::GetObject()
{
  return m_pObject;
}

bool LitesqlObjectPanel::TransferData(bool toWindow)
{
 
  transfer_text(m_textCtrlName,m_pObject->name,toWindow);
  transfer_choice(m_choiceInheritsFrom,m_pObject->inherits,toWindow);
  return true;
}

#include "LitesqlObjectPanel.h"
#include "objectmodel.hpp"

using namespace xml;

LitesqlObjectPanel::LitesqlObjectPanel( wxWindow* parent, Object* pObject  )
:
ui::ObjectPanel( parent ),
m_pObject(pObject)
{
  m_textCtrlName->SetValue(pObject->name);
}
 
#include "LitesqlFieldPanel.h"
#include "objectmodel.hpp"

using namespace xml;

LitesqlFieldPanel::LitesqlFieldPanel( wxWindow* parent , Field* pField)
:
FieldPanel( parent ),
m_pField(pField)
{

}

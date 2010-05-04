#include "LitesqlRelationPanel.h"
#include "objectmodel.hpp"

using namespace xml;

LitesqlRelationPanel::LitesqlRelationPanel( wxWindow* parent, Relation* pRelation )
: ui::RelationPanel( parent ),
  m_pRelation(pRelation)
{

}

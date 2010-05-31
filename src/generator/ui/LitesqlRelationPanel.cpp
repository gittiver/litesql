#include "LitesqlRelationPanel.h"
#include "objectmodel.hpp"

using namespace xml;

LitesqlRelationPanel::LitesqlRelationPanel( wxWindow* parent, Relation* pRelation )
: ui::RelationPanel( parent ),
  m_pRelation(pRelation)
{

}

bool LitesqlRelationPanel::TransferData(bool toWindow)
{
  TRANSFER_TEXT(m_textCtrlName,m_pRelation->name,toWindow);
  return true;
}

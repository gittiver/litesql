#include "LitesqlDatabasePanel.h"
#include "xmlObjects.hpp"

using namespace ui;
using namespace xml;

LitesqlDatabasePanel::LitesqlDatabasePanel( wxWindow* parent,Database* db )
:
DatabasePanel( parent ),
m_database(db)
{}

bool LitesqlDatabasePanel::TransferData(bool toWindow)
{
  transfer_text(m_textName,m_database->name,toWindow);
  transfer_text(m_textInclude,m_database->include,toWindow);
  transfer_text(m_textNamespace,m_database->nspace,toWindow);
  return true;
}
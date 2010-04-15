#ifndef LITESQL_DOCUMENT_H
#define LITESQL_DOCUMENT_H

#include <wx/docview.h>

class LitesqlDocument: public wxDocument
{
  DECLARE_DYNAMIC_CLASS(LitesqlDocument)

public:

  LitesqlDocument(void){};
  ~LitesqlDocument(void);
};

#endif // #ifndef LITESQL_DOCUMENT_H

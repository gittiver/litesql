#ifndef LITESQL_DOCUMENT_H
#define LITESQL_DOCUMENT_H

#include <wx/docview.h>
#include "objectmodel.hpp"


class uiField {
private:
  xml::Field* m_pField;
public:
  uiField(xml::Field* pField) : m_pField(pField) {};

  bool isEditable() const;
  xml::Field* field() { return m_pField; };

  bool indexed() const { return m_pField->isIndexed(); }
  void indexed(bool isIndexed) { m_pField->indexed = isIndexed ? A_field_indexed_true:A_field_indexed_false; }
  
  bool unique() const { return m_pField->isUnique(); }
  void unique(bool isUnique) { m_pField->unique = isUnique ? A_field_unique_true:A_field_unique_false; }

  wxString type() const { return litesql::toString(m_pField->type); }
  void type(const wxString& type) { m_pField->type = litesql::field_type(type); }

  static const wxArrayString FIELDTYPES;

protected:
  virtual ~uiField() {};

};

class LitesqlDocument: public wxDocument
{
  DECLARE_DYNAMIC_CLASS(LitesqlDocument)

public:

  LitesqlDocument(void);
  virtual ~LitesqlDocument(void);

  litesql::ObjectModel* GetModel();

protected:

#if wxUSE_STD_IOSTREAM
  wxSTD ostream& SaveObject(wxSTD ostream& text_stream);
  wxSTD istream& LoadObject(wxSTD istream& text_stream);
#else
  wxOutputStream& SaveObject(wxOutputStream& stream);
  wxInputStream& LoadObject(wxInputStream& stream);
#endif

private:
  litesql::ObjectModel* m_pModel; 
};

#endif // #ifndef LITESQL_DOCUMENT_H

#ifndef _litesql_xmlparser_hpp
#define _litesql_xmlparser_hpp

#include <string>

#include <libxml/parser.h>

/** abstract base class for SAX2 Parser based on libxml2 */
class XmlParser {

public:
   bool parseFile(const std::string& filename);
   
   static const xmlChar* xmlGetAttrValue(const xmlChar** attrs,const char* key);
   static const xmlChar* xmlGetAttrValue(const xmlChar** attrs,const xmlChar* key);

protected:
   virtual void onStartElement(	const xmlChar *fullname,
                                 const xmlChar **atts)	= 0;
   virtual void onEndElement(	const xmlChar *fullname)   = 0;

private:

   friend void XMLParser_xmlSAX2StartElement		(void *ctx,
						 const xmlChar *fullname,
						 const xmlChar **atts);

   friend void XMLParser_xmlSAX2EndElement(void *ctx,const xmlChar *name);
};

#endif


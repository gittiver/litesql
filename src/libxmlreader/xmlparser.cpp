#include <iostream>

#include "xmlparser.hpp"

using namespace std;

class SaxHandler4XmlParser {
public:
   SaxHandler4XmlParser()
   {
      memset(&saxHandler,0,sizeof(saxHandler));
      saxHandler.startElement = &XMLParser_xmlSAX2StartElement;
      saxHandler.endElement = &XMLParser_xmlSAX2EndElement;
   };

   _xmlSAXHandler saxHandler;
};

/**one (initialized) instance for class XmlParser */
SaxHandler4XmlParser saxHandler4XmlParser; 

void XMLParser_xmlSAX2StartElement		(void *ctx,
						 const xmlChar *fullname,
						 const xmlChar **atts)
{
   ((XmlParser*)ctx)->onStartElement(fullname,atts);
}

void XMLParser_xmlSAX2EndElement(void *ctx,const xmlChar *name)
{
   ((XmlParser*)ctx)->onEndElement(name);
}


bool XmlParser::parseFile(const std::string& filename)
{
   int result = xmlSAXUserParseFile(&saxHandler4XmlParser.saxHandler,this,filename.c_str());
   if (result!=0)
   {
      cerr << "error parsing " << filename.c_str() << endl;
   }
   return result == 0;
}

const xmlChar* XmlParser::xmlGetAttrValue(const xmlChar** attrs,const char* key)
{
   return xmlGetAttrValue(attrs,(xmlChar*) key);
}

const xmlChar* XmlParser::xmlGetAttrValue(const xmlChar** attrs,const xmlChar* key)
{
   if (attrs!=NULL)
   {      
      for (size_t i = 0; attrs[i]!=NULL;i+=2)
      {
         if (xmlStrEqual(attrs[i],key))
         {
            return attrs[i+1];
         } 
      }
   }
   return NULL;
}

#!/usr/bin/python

import lxml.etree as etree

from StringIO import StringIO 

import litesqlxml

xsd_doc = etree.parse(open("xsd.xsd"))
xsd = etree.XMLSchema(xsd_doc)

rng_doc = etree.parse(open("litesql.xsd"))
print xsd.validate(rng_doc)
log = xsd.error_log
print  log.last_error
#rng_doc = etree.parse(open("litesql.rng"))
#rng = etree.RelaxNG(rng_doc)

rng = etree.XMLSchema(rng_doc)

doc = etree.parse(open("database.xml"))
doc.xinclude()
litesqlxml.prepare(doc)    
print rng.validate(doc)
log = rng.error_log
print log.last_error

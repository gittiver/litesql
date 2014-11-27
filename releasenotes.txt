Version 0.3.17
##############
* fix for #39 MD5 hash bug, UINT4 is now an uint32_t, which should work also correct on 64bit systems
* updated examples
* applied patch #15 without changes for mysql - added protobuf generator
* removed Blob::nil (patch #13)
* parser error fixed for relation::field::values (values inside relations are not parsed before)
* bugfix for #36 fatal error: exampledatabase.hpp: No such file or directory 
* bugfix for #34 Missing constants for values in fields of relations
* applied suggested patch #12 MySQL Connector C /NODEFAULTLIB:LIBCMT 

Version 0.3.16
##############
- bugfix for #31 "delRelations not called on delete record" 

Version 0.3.15
##############
- bugfix and extension for blobs

Version 0.3.14
##############
- bugfix for memory leak on sqlite-backend (https://sourceforge.net/tracker/?func=detail&aid=3531292&group_id=113304&atid=664757) 

Version 0.3.13
##############
- support for oracle backend (ocilib) 
- database specific datatypes are used (needed for oracle backend)

Version 0.3.12
##############
- Visual Litesql Link installed in Menu
- fixed error in codegenerator factory map in it on linux
- codegenerator for wxformbuilder added (Initial version)
- autotools support updated

Version 0.3.11
##############
- support for bigint datatype

Version 0.3.10
##############
- better gui
- changed memory handling inside model (counted_ptr)
- fixes in autoconf-based build

Version 0.3.9
#############
- platform-specific build issues fixed
- fix for xml-parser error on method params
- made function parameters on runtime classe const
- usable GUI-Tool (not all features of litesql are supported, 
  ui design could be better, on OSX the gui is terrible layouted )  

Version 0.3.8
#############
- added double as field type for higher precision calculation
- fixed trac ticket #25 (uncatched exceptions on validating model are 
  catched now) 
- Beta for Gui-Tool for editing and generating Code (cross-platform through wxWidgets)

Version 0.3.7
#############
- fixed error on creating indexes (see trac ticket #23)
Version 0.3.6
#############

- moved code for loading and storing the model into an own library

- added initial version of a win32 gui for code generation (not ready for production use)

- refactored code generators
- added code generator for ActiveRecord (not ready for production use)
- added multi-target code generation ( multiple -t options)
- added options to litesql-gen for output-directory (--output-dir=)
 
- fixed a bug with postgres on database update 
- fixed a bug: added parsing of index and indexfield tags (was not in the new parser)

/* XML processor/application API for litesql.dtd.
 * Generated 2007/02/10 23:31:23.
 *
 * This program was generated with the FleXML XML processor generator.
 * FleXML is Copyright (C) 1999-2005 Kristoffer Rose.  All rights reserved.
 * FleXML is Copyright (C) 2003-2006 Martin Quinson.  All rights reserved.
 * (Id: flexml.pl,v 1.61 2006/09/13 16:34:33 wdowling Exp).
 * 
 * There are two, intertwined parts to this program, part A and part B.
 *
 * Part A
 * ------
 * 
 * Some parts, here collectively called "Part A", are found in the 
 * FleXML package.  They are Copyright (C) 1999-2005 Kristoffer Rose
 * and Copyright (C) 2003-2006 Martin Quinson. All rights reserved.
 *
 * You can redistribute, use, perform, display and/or modify "Part A"
 * provided the following two conditions hold:
 *
 * 1. The program is distributed WITHOUT ANY WARRANTY from the author of
 *    FleXML; without even the implied warranty of MERCHANTABILITY or
 *    FITNESS FOR A PARTICULAR PURPOSE.
 *
 * 2. The program distribution conditions do not in any way affect the
 *    distribution conditions of the FleXML system used to generate this
 *    file or any version of FleXML derived from that system.
 *
 * Notice that these are explicit rights granted to you for files
 * generated by the FleXML system.  For your rights in connection with
 * the FleXML system itself please consult the GNU General Public License.
 * 
 * Part B
 * ------
 * 
 * The other parts, here collectively called "Part B", and which came 
 * from the DTD used by FleXML to generate this program, can be 
 * distributed (or not, as the case may be) under the terms of whoever
 * wrote them, provided these terms respect and obey the two conditions 
 * above under the heading "Part A".
 *
 * The author of and contributors to FleXML specifically disclaim
 * any copyright interest in "Part B", unless "Part B" was written 
 * by the author of or contributors to FleXML.
 * 
 */

#ifndef _FLEXML_litesql_H
#define _FLEXML_litesql_H

/* XML application entry points. */
extern void STag_database(void);
extern void ETag_database(void);
extern void STag_object(void);
extern void ETag_object(void);
extern void STag_interface(void);
extern void ETag_interface(void);
extern void STag_implements(void);
extern void ETag_implements(void);
extern void STag_check(void);
extern void ETag_check(void);
extern void STag_type(void);
extern void ETag_type(void);
extern void STag_represent(void);
extern void ETag_represent(void);
extern void STag_store(void);
extern void ETag_store(void);
extern void STag_include(void);
extern void ETag_include(void);
extern void STag_option(void);
extern void ETag_option(void);
extern void STag_field(void);
extern void ETag_field(void);
extern void STag_index(void);
extern void ETag_index(void);
extern void STag_indexfield(void);
extern void ETag_indexfield(void);
extern void STag_value(void);
extern void ETag_value(void);
extern void STag_method(void);
extern void ETag_method(void);
extern void STag_param(void);
extern void ETag_param(void);
extern void STag_relation(void);
extern void ETag_relation(void);
extern void STag_relate(void);
extern void ETag_relate(void);

/* XML application data. */
typedef int AT_type_name;
#define AU_type_name NULL
typedef int AT_option_name;
#define AU_option_name NULL
typedef int AT_relation_id;
#define AU_relation_id NULL
typedef enum { AU_relate_limit, A_relate_limit_one,A_relate_limit_many } AT_relate_limit;
typedef int AT_object_name;
#define AU_object_name NULL
typedef int AT_interface_name;
#define AU_interface_name NULL
typedef enum { AU_field_indexed, A_field_indexed_true,A_field_indexed_false } AT_field_indexed;
typedef int AT_database_name;
#define AU_database_name NULL
typedef int AT_database_namespace;
#define AU_database_namespace NULL
typedef enum { AU_relation_unidir, A_relation_unidir_true,A_relation_unidir_false } AT_relation_unidir;
typedef int AT_check_function;
#define AU_check_function NULL
typedef int AT_method_name;
#define AU_method_name NULL
typedef int AT_represent_as;
#define AU_represent_as NULL
typedef int AT_field_default;
#define AU_field_default NULL
typedef int AT_method_returntype;
#define AU_method_returntype NULL
typedef int AT_param_type;
#define AU_param_type NULL
typedef int AT_relate_remotehandle;
#define AU_relate_remotehandle NULL
typedef int AT_represent_target;
#define AU_represent_target NULL
typedef int AT_indexfield_name;
#define AU_indexfield_name NULL
typedef int AT_check_param;
#define AU_check_param NULL
typedef int AT_option_value;
#define AU_option_value NULL
typedef int AT_value_name;
#define AU_value_name NULL
typedef int AT_relate_object;
#define AU_relate_object NULL
typedef int AT_database_include;
#define AU_database_include NULL
typedef int AT_store_backend;
#define AU_store_backend NULL
typedef enum { AU_object_temporary, A_object_temporary_true,A_object_temporary_false } AT_object_temporary;
typedef int AT_field_name;
#define AU_field_name NULL
typedef enum { AU_relate_unique, A_relate_unique_true,A_relate_unique_false } AT_relate_unique;
typedef int AT_object_inherits;
#define AU_object_inherits NULL
typedef int AT_value_value;
#define AU_value_value NULL
typedef enum { AU_method_const, A_method_const_true,A_method_const_false } AT_method_const;
typedef int AT_relate_interface;
#define AU_relate_interface NULL
typedef int AT_store_as;
#define AU_store_as NULL
typedef int AT_option_backend;
#define AU_option_backend NULL
typedef enum { AU_index_unique, A_index_unique_true,A_index_unique_false } AT_index_unique;
typedef int AT_param_name;
#define AU_param_name NULL
typedef int AT_field_type;
#define AU_field_type NULL
typedef int AT_include_file;
#define AU_include_file NULL
typedef int AT_relation_name;
#define AU_relation_name NULL
typedef int AT_relate_handle;
#define AU_relate_handle NULL
typedef enum { AU_field_unique, A_field_unique_true,A_field_unique_false } AT_field_unique;
typedef int AT_implements_interface;
#define AU_implements_interface NULL

/* FleXML-provided data. */
extern int pcdata_ix;
extern char *bufferstack;
#define pcdata (bufferstack + pcdata_ix)
extern AT_type_name AX_type_name;
#define A_type_name (bufferstack + AX_type_name)
extern AT_option_name AX_option_name;
#define A_option_name (bufferstack + AX_option_name)
extern AT_relation_id AX_relation_id;
#define A_relation_id (bufferstack + AX_relation_id)
extern AT_relate_limit AX_relate_limit;
#define A_relate_limit AX_relate_limit
extern AT_object_name AX_object_name;
#define A_object_name (bufferstack + AX_object_name)
extern AT_interface_name AX_interface_name;
#define A_interface_name (bufferstack + AX_interface_name)
extern AT_field_indexed AX_field_indexed;
#define A_field_indexed AX_field_indexed
extern AT_database_name AX_database_name;
#define A_database_name (bufferstack + AX_database_name)
extern AT_database_namespace AX_database_namespace;
#define A_database_namespace (bufferstack + AX_database_namespace)
extern AT_relation_unidir AX_relation_unidir;
#define A_relation_unidir AX_relation_unidir
extern AT_check_function AX_check_function;
#define A_check_function (bufferstack + AX_check_function)
extern AT_method_name AX_method_name;
#define A_method_name (bufferstack + AX_method_name)
extern AT_represent_as AX_represent_as;
#define A_represent_as (bufferstack + AX_represent_as)
extern AT_field_default AX_field_default;
#define A_field_default (bufferstack + AX_field_default)
extern AT_method_returntype AX_method_returntype;
#define A_method_returntype (bufferstack + AX_method_returntype)
extern AT_param_type AX_param_type;
#define A_param_type (bufferstack + AX_param_type)
extern AT_relate_remotehandle AX_relate_remotehandle;
#define A_relate_remotehandle (bufferstack + AX_relate_remotehandle)
extern AT_represent_target AX_represent_target;
#define A_represent_target (bufferstack + AX_represent_target)
extern AT_indexfield_name AX_indexfield_name;
#define A_indexfield_name (bufferstack + AX_indexfield_name)
extern AT_check_param AX_check_param;
#define A_check_param (bufferstack + AX_check_param)
extern AT_option_value AX_option_value;
#define A_option_value (bufferstack + AX_option_value)
extern AT_value_name AX_value_name;
#define A_value_name (bufferstack + AX_value_name)
extern AT_relate_object AX_relate_object;
#define A_relate_object (bufferstack + AX_relate_object)
extern AT_database_include AX_database_include;
#define A_database_include (bufferstack + AX_database_include)
extern AT_store_backend AX_store_backend;
#define A_store_backend (bufferstack + AX_store_backend)
extern AT_object_temporary AX_object_temporary;
#define A_object_temporary AX_object_temporary
extern AT_field_name AX_field_name;
#define A_field_name (bufferstack + AX_field_name)
extern AT_relate_unique AX_relate_unique;
#define A_relate_unique AX_relate_unique
extern AT_object_inherits AX_object_inherits;
#define A_object_inherits (bufferstack + AX_object_inherits)
extern AT_value_value AX_value_value;
#define A_value_value (bufferstack + AX_value_value)
extern AT_method_const AX_method_const;
#define A_method_const AX_method_const
extern AT_relate_interface AX_relate_interface;
#define A_relate_interface (bufferstack + AX_relate_interface)
extern AT_store_as AX_store_as;
#define A_store_as (bufferstack + AX_store_as)
extern AT_option_backend AX_option_backend;
#define A_option_backend (bufferstack + AX_option_backend)
extern AT_index_unique AX_index_unique;
#define A_index_unique AX_index_unique
extern AT_param_name AX_param_name;
#define A_param_name (bufferstack + AX_param_name)
extern AT_field_type AX_field_type;
#define A_field_type (bufferstack + AX_field_type)
extern AT_include_file AX_include_file;
#define A_include_file (bufferstack + AX_include_file)
extern AT_relation_name AX_relation_name;
#define A_relation_name (bufferstack + AX_relation_name)
extern AT_relate_handle AX_relate_handle;
#define A_relate_handle (bufferstack + AX_relate_handle)
extern AT_field_unique AX_field_unique;
#define A_field_unique AX_field_unique
extern AT_implements_interface AX_implements_interface;
#define A_implements_interface (bufferstack + AX_implements_interface)

/* XML application utilities. */
extern int element_context(int);

/* XML processor entry point. */
extern int yylex(void);

/* Flexml error handling function (useful only when -q flag passed to flexml) */
const char * parse_err_msg(void);
#endif

/* LiteSQL 
 * 
 * The list of contributors at http://litesql.sf.net/ 
 * 
 * See LICENSE for copyright information. */
#ifndef _litesql_types_hpp
#define _litesql_types_hpp
#include <string>
#include <vector>
#include "litesql/utils.hpp"
#include "litesql/except.hpp"
/** \file types.hpp
    contains class Record and typedef Records */

namespace litesql
{
class Database;
/** SQL data row wrapper. */
class Record : public std::vector<std::string>
{
public: 
    Record() : std::vector<std::string>() {}
    Record(size_t reserved) { reserve(reserved); }
};
/** shortcut */
typedef std::vector<Record> Records;

}
typedef enum { AU_field_type, 
               A_field_type_boolean,
               A_field_type_integer,
               A_field_type_bigint,
               A_field_type_string,
               A_field_type_float,
               A_field_type_double,
               A_field_type_time,
               A_field_type_date,
               A_field_type_datetime,
               A_field_type_blob 
             } AT_field_type;

#endif

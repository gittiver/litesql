#ifndef commontypes_h
#define commontypes_h

typedef enum { AU_relate_limit, A_relate_limit_one,A_relate_limit_many } AT_relate_limit;
typedef enum { AU_field_indexed, A_field_indexed_true,A_field_indexed_false } AT_field_indexed;
typedef enum { AU_relate_unique, A_relate_unique_true,A_relate_unique_false } AT_relate_unique;
typedef enum { AU_relation_unidir, A_relation_unidir_true,A_relation_unidir_false } AT_relation_unidir;
typedef enum { AU_index_unique, A_index_unique_true,A_index_unique_false } AT_index_unique;
typedef enum { AU_field_unique, A_field_unique_true,A_field_unique_false } AT_field_unique;

typedef enum { 
  AU_field_type, 
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

const char* toAttributeString(AT_field_type t);

#endif // #ifndef commontypes_h


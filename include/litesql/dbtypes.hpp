/* LiteSQL 
 * 
 * By Tero Laitinen 
 * 
 * See LICENSE for copyright information. */

#ifndef _litesql_dbtypes_hpp
#define _litesql_dbtypes_hpp

#include <string>
#include <map>
#include <list>

namespace litesql {
    namespace db {

        class Type {
        public:
            typedef std::string BackendName;
            typedef std::string SQLType;
            typedef std::map< BackendName, SQLType > SQLTypes;

        private:
            std::string name;
            SQLTypes sqlTypes;

        public:            

            Type(const std::string& name, 
                 const SQLTypes& sqlTypes);
        };

        class Table;

        class Field {

            std::string name_;
            Type* type_;
            Table* table_;
            bool unique_;
            Field* references_; 

        public:

            Field(const std::string& name,
                  const std::string& type,
                  Table* table,
                  bool unique,
                  Field* references);

            std::string name() const;
            Type* type() const;
            Table* table() const;
            bool unique() const;
            Field* references() const;
        };

        class Table {

            std::string name_;
            std::list<Field*> fields;

        public:

            Table(const std::string& name);
            std::string name() const;
            void addField(Field* field);

        };

        class Index {

            std::string name;
            Table* table;
            std::list<Field*> fields;

        public:
            Index(const std::string& name);
            void addField(Field* field);
        };

        class Sequence {

            std::string name;

        public:

            Sequence(const std::string& name);
        };

        class Database {
        public:
            typedef std::list<Table*> Tables;
            typedef std::list<Index*> Indices;
            typedef std::list<Sequence*> Sequences;

        private:
            std::string name;
            Tables tables;
            Indices indices;
            Sequences sequences;

        public:
            Database(const std::string& name,
                     const Tables& tables,
                     const Indices& indices,
                     const Sequences& sequences);
        };

    }

}
#endif

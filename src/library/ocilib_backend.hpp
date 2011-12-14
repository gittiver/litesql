/* LiteSQL 
 * 
 * The list of contributors at http://litesql.sf.net/ 
 * 
 * See LICENSE for copyright information. */

/** \file ocilib_backend.hpp
 * ocilib_backend */
#ifndef litesql_ocilib_backend_hpp
#define litesql_ocilib_backend_hpp

#ifdef HAVE_OCILIB


#include "litesql/except.hpp"
#include "litesql/types.hpp"
#include "litesql/string.hpp"
#include "litesql/backend.hpp"

typedef struct OCI_Connection OCI_Connection;

namespace litesql {

class OCILib : public Backend {
	OCI_Connection *conn;
	// database is same as ora service
	std::string user, passwd, database;
	static bool oci_init;
public:
  class Cursor;
  class Result;

  OCILib(const std::string& connInfo);
    virtual ~OCILib();
	virtual string getTextType() const {
				return "VARCHAR(4000)";
			}
	virtual string getCreateSequenceSQL(const string& name) const;
    virtual string getSeqSQL(const string& sname) const;
    /* virtual */ string  getSQLType(AT_field_type fieldType) const;
	//virtual string queryTerm() const { return "";}
    virtual bool supportsSequences() const;
    virtual void begin() const;
    virtual void commit() const;
    virtual void rollback() const;
  Backend::Result* execute(const std::string& query) const;
  Backend::Cursor* cursor(const std::string& query) const;
protected:
	void checkError() const;
};
}
#endif // #ifdef HAVE_OCILIB
#endif // #ifndef litesql_ocilib_backend_hpp


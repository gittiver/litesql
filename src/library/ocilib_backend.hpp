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
	static bool oci_init;
public:
  class Cursor;
  class Result;

  OCILib(const std::string& connInfo);
    virtual ~OCILib();
	virtual std::string getCreateSequenceSQL(const std::string& name) const;
    virtual std::string getSeqSQL(const std::string& sname) const;
    /* virtual */ std::string  getSQLType(AT_field_type fieldType,const std::string& length="") const;
	//virtual string queryTerm() const { return "";}
    virtual bool supportsSequences() const;
    virtual void begin() const;
    virtual void commit() const;
    virtual void rollback() const;
  Backend::Result* execute(const std::string& query) const;
  Backend::Cursor* cursor(const std::string& query) const;

  static bool verbose;

protected:
	std::string checkError(bool shouldThrow=true) const;
};
}
#endif // #ifdef HAVE_OCILIB
#endif // #ifndef litesql_ocilib_backend_hpp


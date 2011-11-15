/* LiteSQL - OCILib-backend implementation
* 
* The list of contributors at http://litesql.sf.net/ 
* 
* See LICENSE for copyright information. */
#include "config.h"
#include "ocilib_backend.hpp"

#include <string>

#ifdef HAVE_OCILIB

#ifdef WIN32
#include <winsock2.h>				/* For windows */
#endif

#include "ocilib.h"

using namespace litesql;
using namespace std;

#include "litesql/field.hpp"


/** OCILib - Cursor */
class OCILib::Cursor : public Backend::Cursor {
protected:
	OCI_Resultset * res;
	OCI_Statement  *st;
	std::vector<unsigned int> cTypes; // column types
public:
	Cursor(OCI_Resultset * _res, OCI_Statement  *_st);
	virtual ~Cursor();
	virtual void setCacheSize(int s);
	virtual Record fetchOne(){ return fetchOneRow();}
	virtual Record fetchOneRow() const;
};

OCILib::Cursor::Cursor(OCI_Resultset * _res, OCI_Statement  *_st)
: res(_res),st(_st) 
{
	unsigned int nFields = (!res) ? 0 : OCI_GetColumnCount(res);
	cTypes.reserve(nFields);
	for(unsigned int c=1;c<=nFields;c++)
	{
		cTypes.push_back(OCI_ColumnGetType(OCI_GetColumn(res,c)));
	}
}
OCILib::Cursor::~Cursor() {
	if(st)
		OCI_FreeStatement(st);
}
void OCILib::Cursor::setCacheSize(int s) {
	if(res)
	{
		OCI_SetFetchSize(st,s);
	}
}

Record OCILib::Cursor::fetchOneRow() const{
	if(!OCI_FetchNext(res))
	{
		return Record(); //done
	}

	unsigned int nFields =cTypes.size();
	Record row(nFields);
	for (size_t i2 = 1; i2 <= nFields; i2++) 
	{
		unsigned int colType=cTypes[i2-1];
		if(colType==OCI_CDT_LOB)
		{
			OCI_Lob *l= OCI_GetLob(res,i2);
			if(l)
			{
				big_uint len=OCI_LobGetLength(l);
				if(len>=big_uint(INT_MAX))
				{
					throw MemoryError("Not supporting really big LOBs");
				}
				size_t blen=(size_t)len;
				size_t clen=0;
				litesql::u8_t *buff = (litesql::u8_t *)malloc(len+1);
				if(OCI_LobRead2(l,buff,&clen,&blen))
				{
					row.push_back(litesql::Blob::toHex(buff,blen));
				}
				else row.push_back("NULL");
			}
			else row.push_back("NULL");
		}
		else if(colType==OCI_CDT_TEXT)
		{
			const char *val=OCI_GetString(res,i2);
			row.push_back((val) ? val : "");
		}
		else 
		{
			const char *val=OCI_GetString(res,i2);
			row.push_back((val) ? val : "NULL");
		}
	}

	return row;
}

/** OCILib - result */
class OCILib::Result : public Backend::Result, public OCILib::Cursor {
public:
	Result(OCI_Resultset* _res,OCI_Statement  *_st) : OCILib::Cursor(_res,_st){     };
	virtual ~Result();
	virtual size_t fieldNum() const;
	virtual Record fields() const;
	virtual size_t recordNum() const;
	virtual Records records() const;
};

OCILib::Result::~Result() {

}
size_t OCILib::Result::fieldNum() const {
	return cTypes.size();    
}

Record OCILib::Result::fields() const {
	if (!res)
		return Record();

	unsigned int nFields =cTypes.size();
	Record rec(nFields);
	for (size_t i = 0; i < nFields ; i++)
		rec.push_back(OCI_ColumnGetName(OCI_GetColumn(res,i)));
	return rec;
}
//The following never works... apparently we do not know afore hand how many rows
//we are going to fetch :(
size_t OCILib::Result::recordNum() const {
	if (!res)
		return 0;
	return OCI_GetRowCount(res);
}  
Records OCILib::Result::records() const {
	if (!res)
		return Records();
	Records recs;
	while(true)
	{
		Record r=OCILib::Cursor::fetchOneRow();
		if(r.empty()) 
			break;
		recs.push_back(r);
	}
	return recs;
}



bool OCILib::oci_init=false;
void err_handler(OCI_Error *err)
{
	int   err_type = OCI_ErrorGetType(err);
	const char *err_msg  = OCI_ErrorGetString(err);
	printf("%s - %s\n", err_type == OCI_ERR_WARNING ? "warning" : "error", err_msg);
}

void OCILib::checkError() const
{
	OCI_Error *err=OCI_GetLastError();
	if(err){
		int   err_type = OCI_ErrorGetType(err);
		const char *err_msg  = OCI_ErrorGetString(err);
		int ocode=OCI_ErrorGetOCICode(err);
		if(err_type != OCI_ERR_WARNING && ocode!=1408)
		{
			throw DatabaseError(err_msg);
		}
	}
}
OCILib::OCILib(const std::string& connInfo):conn(0) {
	Split params(connInfo,";");
	for (size_t i = 0; i < params.size(); i++) {
		Split param(params[i], "=");
		if (param.size() == 1)
			continue;
		if (param[0] == "database")
			database = param[1];
		else if (param[0] == "password")
			passwd = param[1];
		else if (param[0] == "user")
			user = param[1];
	}
	if(!oci_init)
	{
		if (!OCI_Initialize(err_handler, 0, OCI_ENV_DEFAULT|OCI_ENV_THREADED|OCI_ENV_CONTEXT ))
		{
			throw DatabaseError("OCILib initialization failure");
		}
		OCI_EnableWarnings(TRUE);
	}
	conn = OCI_ConnectionCreate((database.empty())?0:database.c_str(), user.c_str(), passwd.c_str(), OCI_SESSION_DEFAULT);
	if(!conn)
	{
		printf("error with %s; %s",database.c_str(),user.c_str());
		checkError();
	}
}

OCILib::~OCILib() {
	if(conn)
		OCI_ConnectionFree(conn);
}

bool OCILib::supportsSequences() const {
	return true;
}
void OCILib::begin() const {
	// do nothing
}
void OCILib::commit() const {
	if(!OCI_Commit(conn))
		checkError();
}
void OCILib::rollback() const {
	if(!OCI_Rollback(conn))
		checkError();
}

Backend::Result* OCILib::execute(const string& query) const {
	OCI_Statement  *st= OCI_StatementCreate(conn);
	if(OCI_ExecuteStmt(st,query.c_str()))
	{
		return new Result(OCI_GetResultset(st),st);
	}
	else
	{
		OCI_FreeStatement(st);
		checkError();
	}
	return new Result(0,0); // never come here.
}
Backend::Cursor* OCILib::cursor(const string& query) const {
	OCI_Statement  *st= OCI_StatementCreate(conn);
	if(OCI_ExecuteStmt(st,query.c_str()))
	{
		return new Cursor(OCI_GetResultset(st),st);
	}
	else
	{
		OCI_FreeStatement(st);
		checkError();
	}
	return new Cursor(0,0); // never come here.
}


string OCILib::getSeqSQL(const string& sname) const{
	string ret="SELECT ";
	ret=ret+ sname + ".NEXTVAL FROM DUAL";
	return ret;
}
#endif

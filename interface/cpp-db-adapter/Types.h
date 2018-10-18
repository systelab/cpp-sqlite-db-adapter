#ifndef _DBADAPTERINTERFACE_TYPES_JOSEP_VILA_1712150913_H
#define _DBADAPTERINTERFACE_TYPES_JOSEP_VILA_1712150913_H

namespace systelab { namespace db {

	enum FieldTypes
	{
		BOOLEAN,
		INT,
		DOUBLE,
		STRING,
		DATETIME,
		BINARY
	};

	typedef int RowsAffected;

	typedef long RowId;
}}

#endif //_DBADAPTERINTERFACE_TYPES_JOSEP_VILA_1712150913_H
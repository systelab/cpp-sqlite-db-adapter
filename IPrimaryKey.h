#ifndef _DBADAPTERINTERFACE_IPRIMARYKEY_QV_2502161128_H
#define _DBADAPTERINTERFACE_IPRIMARYKEY_QV_2502161128_H

#include <string>

namespace systelab { namespace db {

	class ITable;
	class IField;

	class IPrimaryKey
	{
	public:
		virtual ~IPrimaryKey() {}

		virtual ITable& getTable() const = 0;

		virtual unsigned int getFieldsCount() const = 0;
		virtual const IField& getField(unsigned int index) const = 0;
		virtual const IField& getField(const std::string& fieldName) const = 0;
	};

}}

#endif // _DBADAPTERINTERFACE_IPRIMARYKEY_QV_2502161128_H
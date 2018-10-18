#ifndef _DBADAPTERINTERFACE_ITABLERECORD_QV_0303161111_H
#define _DBADAPTERINTERFACE_ITABLERECORD_QV_0303161111_H

#include <string>
#include <vector>

namespace systelab { namespace db {

	class ITable;
	class IField;
	class IFieldValue;

	class ITableRecord
	{
	public:
		virtual ~ITableRecord() {}

		virtual ITable& getTable() const = 0;

		virtual unsigned int getFieldValuesCount() const = 0;
		virtual IFieldValue& getFieldValue(unsigned int index) const = 0;
		virtual IFieldValue& getFieldValue(const std::string& fieldName) const = 0;

		virtual bool hasFieldValue(const std::string& fieldName) const = 0;

		virtual std::vector<IFieldValue*> getValuesList() const = 0;
	};
}}

#endif //_DBADAPTERINTERFACE_ITABLERECORD_QV_0303161111_H
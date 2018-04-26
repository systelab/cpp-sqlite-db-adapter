#ifndef _DBADAPTERINTERFACE_IRECORD_JOSEP_VILA_1712151142_H
#define _DBADAPTERINTERFACE_IRECORD_JOSEP_VILA_1712151142_H

#include "IField.h"
#include "IFieldValue.h"

namespace systelab { namespace db {

	class ITable;

	class IRecord
	{
	public:
		virtual ~IRecord() {}

		virtual unsigned int getFieldValuesCount() const = 0;
		virtual IFieldValue& getFieldValue(unsigned int index) const = 0;
		virtual IFieldValue& getFieldValue(const std::string& fieldName) const = 0;

		virtual bool hasFieldValue(const std::string& fieldName) const = 0;
	};
}}

#endif //_DBADAPTERINTERFACE_IRECORD_JOSEP_VILA_1712151142_H
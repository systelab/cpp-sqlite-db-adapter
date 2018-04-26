#ifndef _DBADAPTERTESTUTILITIES_STUBFIELD_DMC_0410161846_H
#define _DBADAPTERTESTUTILITIES_STUBFIELD_DMC_0410161846_H

#include "DbAdapterInterface/IField.h"

namespace systelab { namespace db {
	class IBinaryValue;
}}

namespace systelab { namespace test_utility {

	class StubField : public db::IField
	{
	public:
		StubField(const db::IField&);
		StubField(const StubField& other);
		StubField(const std::string& name, db::FieldTypes type);
		StubField(unsigned int index, const std::string& name, db::FieldTypes type, const std::string& defaultValue, bool primaryKey);
		virtual ~StubField();

		unsigned int getIndex() const;
		std::string getName() const;
		db::FieldTypes getType() const;

		bool hasNullDefaultValue() const;
		bool getBooleanDefaultValue() const;
		int getIntDefaultValue() const;
		double getDoubleDefaultValue() const;
		std::string getStringDefaultValue() const;
		boost::posix_time::ptime getDateTimeDefaultValue() const;
		db::IBinaryValue& getBinaryDefaultValue() const;

		bool isPrimaryKey() const;

		StubField& operator= (const StubField& other);

	private:
		unsigned int m_index;
		std::string m_name;
		db::FieldTypes m_type;
		bool m_primaryKey;

		bool m_nullDefaultValue;
		bool m_defaultBoolValue;
		int m_defaultIntValue;
		double m_defaultDoubleValue;
		std::string m_defaultStringValue;
		boost::posix_time::ptime m_defaultDateTimeValue;

		void setDefaultValue(db::FieldTypes type, const std::string& defaultValue);
		boost::posix_time::ptime getDateTimeFromSQLiteString(const std::string& sqliteDateTime) const;
	};

}}

#endif //_DBADAPTERTESTUTILITIES_STUBFIELD_DMC_0410161846_H
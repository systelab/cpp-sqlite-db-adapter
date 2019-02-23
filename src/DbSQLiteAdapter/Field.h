#ifndef _DBSQLITEADAPTER_FIELD_QUIM_VILA_2112151646_H
#define _DBSQLITEADAPTER_FIELD_QUIM_VILA_2112151646_H

#include "DbAdapterInterface/IField.h"

#include "DbAdapterInterface/IBinaryValue.h"


namespace systelab { namespace db { namespace sqlite {

	class Field : public IField
	{
	public:
		Field(unsigned int index, const std::string& name, FieldTypes type, const std::string& defaultValue, bool primaryKey);
		virtual ~Field();

		unsigned int getIndex() const;
		std::string getName() const;
		FieldTypes getType() const;
		bool isPrimaryKey() const;

		bool hasNullDefaultValue() const;
		bool getBooleanDefaultValue() const;
		int getIntDefaultValue() const;
		double getDoubleDefaultValue() const;
		std::string getStringDefaultValue() const;
		boost::posix_time::ptime getDateTimeDefaultValue() const;
		IBinaryValue& getBinaryDefaultValue() const;

	private:
		unsigned int m_index;
		std::string m_name;
		FieldTypes m_type;
		bool m_primaryKey;

		bool m_nullDefaultValue;
		bool m_defaultBoolValue;
		int m_defaultIntValue;
		double m_defaultDoubleValue;
		std::string m_defaultStringValue;
		boost::posix_time::ptime m_defaultDateTimeValue;
		std::unique_ptr<IBinaryValue> m_defaultBinaryValue;

		void setDefaultValue(FieldTypes type, const std::string& defaultValue);
		boost::posix_time::ptime getDateTimeFromSQLiteString(const std::string& sqliteDateTime) const;
	};

}}}

#endif //_DBSQLITEADAPTER_FIELD_QUIM_VILA_2112151646_H
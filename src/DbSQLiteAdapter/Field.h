#pragma once

#include "DbAdapterInterface/IField.h"

#include "DbAdapterInterface/IBinaryValue.h"


namespace systelab { namespace db { namespace sqlite {

	class Field : public IField
	{
	public:
		Field(unsigned int index, const std::string& name, FieldTypes type, const std::string& defaultValue, bool primaryKey);
		virtual ~Field();

		unsigned int getIndex() const override;
		std::string getName() const override;
		FieldTypes getType() const override;
		bool isPrimaryKey() const override;

		bool hasNullDefaultValue() const override;
		bool getBooleanDefaultValue() const override;
		int getIntDefaultValue() const override;
		double getDoubleDefaultValue() const override;
		std::string getStringDefaultValue() const override;
		DateTimeType getDateTimeDefaultValue() const override;
		IBinaryValue& getBinaryDefaultValue() const override;

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
		DateTimeType m_defaultDateTimeValue;
		std::unique_ptr<IBinaryValue> m_defaultBinaryValue;

		void setDefaultValue(FieldTypes type, const std::string& defaultValue);
		DateTimeType getDateTimeFromSQLiteString(const std::string& sqliteDateTime) const;
	};

}}}

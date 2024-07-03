#pragma once

#include "DbAdapterInterface/IFieldValue.h"

#include <memory>

namespace systelab { namespace db { namespace sqlite {

	class FieldValue : public IFieldValue
	{
	public:

		FieldValue(const IField&);
		FieldValue(const IField&, bool);
		FieldValue(const IField&, int);
		FieldValue(const IField&, double);
		FieldValue(const IField&, const std::string&);
		FieldValue(const IField&, const DateTimeType&);
		FieldValue(const IField&, std::unique_ptr<IBinaryValue>);
		~FieldValue();

		const IField& getField() const override;

		bool isNull() const override;
		bool isDefault() const override;
		bool getBooleanValue() const override;
		int getIntValue() const override;
		double getDoubleValue() const override;
		std::string getStringValue() const override;
		DateTimeType getDateTimeValue() const override;
		IBinaryValue& getBinaryValue() const override;

		void setValue(const IFieldValue&) override;
		void setNull() override;
		void setDefault() override;
		void setBooleanValue(bool value) override;
		void setIntValue(int value) override;
		void setDoubleValue(double value) override;
		void setStringValue(const std::string& value) override;
		void setDateTimeValue(const DateTimeType& value) override;
		void setBinaryValue(std::unique_ptr<IBinaryValue> value) override;

		void useDefaultValue() override;

		std::unique_ptr<IFieldValue> clone() const override;

	private:
		const IField& m_field;

		bool m_nullValue;
		bool m_default;
		bool m_boolValue;
		int m_intValue;
		double m_doubleValue;
		std::string m_stringValue;
		DateTimeType m_dateTimeValue;
		std::unique_ptr<IBinaryValue> m_binaryValue;
	};

}}}

#ifndef _DBADAPTERTESTUTILITIES_MOCKFIELDVALUE_QV_2201161915_H
#define _DBADAPTERTESTUTILITIES_MOCKFIELDVALUE_QV_2201161915_H

#include "DbAdapterInterface/IFieldValue.h"
#include "DbAdapterInterface/IBinaryValue.h"

#include "MockField.h"

#include <boost/optional.hpp>


namespace systelab { namespace test_utility {

	class MockFieldValue: public db::IFieldValue
	{
	public:
		MockFieldValue()
			:m_mockField(0)
		{
		}

		MockFieldValue(const std::string& name)
		{
			m_mockField = new MockField(name, db::INT);
			EXPECT_CALL(*this, getField()).Times(AnyNumber()).WillRepeatedly(ReturnRef(*m_mockField));
			EXPECT_CALL(*this, isNull()).Times(AnyNumber()).WillRepeatedly(Return(true));
			EXPECT_CALL(*this, isDefault()).Times(AnyNumber()).WillRepeatedly(Return(false));
		}

		MockFieldValue(const std::string& name, bool value)
		{
			m_mockField = new MockField(name, db::BOOLEAN);
			EXPECT_CALL(*this, getField()).Times(AnyNumber()).WillRepeatedly(ReturnRef(*m_mockField));
			EXPECT_CALL(*this, getBooleanValue()).Times(AnyNumber()).WillRepeatedly(Return(value));
			EXPECT_CALL(*this, isNull()).Times(AnyNumber()).WillRepeatedly(Return(false));
			EXPECT_CALL(*this, isDefault()).Times(AnyNumber()).WillRepeatedly(Return(false));
		}

		MockFieldValue(const std::string& name, const boost::optional<bool>& value)
		{
			m_mockField = new MockField(name, db::BOOLEAN);
			EXPECT_CALL(*this, getField()).Times(AnyNumber()).WillRepeatedly(ReturnRef(*m_mockField));
			EXPECT_CALL(*this, isDefault()).Times(AnyNumber()).WillRepeatedly(Return(false));

			if (value)
			{
				EXPECT_CALL(*this, getBooleanValue()).Times(AnyNumber()).WillRepeatedly(Return(value.get()));
				EXPECT_CALL(*this, isNull()).Times(AnyNumber()).WillRepeatedly(Return(false));
			}
			else
			{
				EXPECT_CALL(*this, getBooleanValue()).Times(AnyNumber()).WillRepeatedly(Throw(std::exception("Null value")));
				EXPECT_CALL(*this, isNull()).Times(AnyNumber()).WillRepeatedly(Return(true));
			}
		}

		MockFieldValue(const std::string& name, int value)
		{
			m_mockField = new MockField(name, db::INT);
			EXPECT_CALL(*this, getField()).Times(AnyNumber()).WillRepeatedly(ReturnRef(*m_mockField));
			EXPECT_CALL(*this, getIntValue()).Times(AnyNumber()).WillRepeatedly(Return(value));
			EXPECT_CALL(*this, isNull()).Times(AnyNumber()).WillRepeatedly(Return(false));
			EXPECT_CALL(*this, isDefault()).Times(AnyNumber()).WillRepeatedly(Return(false));
		}

		MockFieldValue(const std::string& name, const boost::optional<int>& value)
		{
			m_mockField = new MockField(name, db::INT);
			EXPECT_CALL(*this, getField()).Times(AnyNumber()).WillRepeatedly(ReturnRef(*m_mockField));
			EXPECT_CALL(*this, isDefault()).Times(AnyNumber()).WillRepeatedly(Return(false));

			if (value)
			{
				EXPECT_CALL(*this, getIntValue()).Times(AnyNumber()).WillRepeatedly(Return(value.get()));
				EXPECT_CALL(*this, isNull()).Times(AnyNumber()).WillRepeatedly(Return(false));
			}
			else
			{
				EXPECT_CALL(*this, getIntValue()).Times(AnyNumber()).WillRepeatedly(Throw(std::exception("Null value")));
				EXPECT_CALL(*this, isNull()).Times(AnyNumber()).WillRepeatedly(Return(true));
			}
		}

		MockFieldValue(const std::string& name, double value)
		{
			m_mockField = new MockField(name, db::DOUBLE);
			EXPECT_CALL(*this, getField()).Times(AnyNumber()).WillRepeatedly(ReturnRef(*m_mockField));
			EXPECT_CALL(*this, getDoubleValue()).Times(AnyNumber()).WillRepeatedly(Return(value));
			EXPECT_CALL(*this, isNull()).Times(AnyNumber()).WillRepeatedly(Return(false));
			EXPECT_CALL(*this, isDefault()).Times(AnyNumber()).WillRepeatedly(Return(false));
		}

		MockFieldValue(const std::string& name, const boost::optional<double>& value)
		{
			m_mockField = new MockField(name, db::DOUBLE);
			EXPECT_CALL(*this, getField()).Times(AnyNumber()).WillRepeatedly(ReturnRef(*m_mockField));
			EXPECT_CALL(*this, isDefault()).Times(AnyNumber()).WillRepeatedly(Return(false));

			if (value)
			{
				EXPECT_CALL(*this, getDoubleValue()).Times(AnyNumber()).WillRepeatedly(Return(value.get()));
				EXPECT_CALL(*this, isNull()).Times(AnyNumber()).WillRepeatedly(Return(false));
			}
			else
			{
				EXPECT_CALL(*this, getDoubleValue()).Times(AnyNumber()).WillRepeatedly(Throw(std::exception("Null value")));
				EXPECT_CALL(*this, isNull()).Times(AnyNumber()).WillRepeatedly(Return(true));
			}
		}

		MockFieldValue(const std::string& name, const std::string& value)
		{
			m_mockField = new MockField(name, db::STRING);
			EXPECT_CALL(*this, getField()).Times(AnyNumber()).WillRepeatedly(ReturnRef(*m_mockField));
			EXPECT_CALL(*this, getStringValue()).Times(AnyNumber()).WillRepeatedly(Return(value));
			EXPECT_CALL(*this, isNull()).Times(AnyNumber()).WillRepeatedly(Return(false));
			EXPECT_CALL(*this, isDefault()).Times(AnyNumber()).WillRepeatedly(Return(false));
		}

		MockFieldValue(const std::string& name, const boost::optional<std::string>& value)
		{
			m_mockField = new MockField(name, db::STRING);
			EXPECT_CALL(*this, getField()).Times(AnyNumber()).WillRepeatedly(ReturnRef(*m_mockField));
			EXPECT_CALL(*this, isDefault()).Times(AnyNumber()).WillRepeatedly(Return(false));

			if (value)
			{
				EXPECT_CALL(*this, getStringValue()).Times(AnyNumber()).WillRepeatedly(Return(value.get()));
				EXPECT_CALL(*this, isNull()).Times(AnyNumber()).WillRepeatedly(Return(false));
			}
			else
			{
				EXPECT_CALL(*this, getStringValue()).Times(AnyNumber()).WillRepeatedly(Throw(std::exception("Null value")));
				EXPECT_CALL(*this, isNull()).Times(AnyNumber()).WillRepeatedly(Return(true));
			}
		}

		MockFieldValue(const std::string& name, const boost::posix_time::ptime& value)
		{
			m_mockField = new MockField(name, db::DATETIME);
			EXPECT_CALL(*this, getField()).Times(AnyNumber()).WillRepeatedly(ReturnRef(*m_mockField));
			EXPECT_CALL(*this, getDateTimeValue()).Times(AnyNumber()).WillRepeatedly(Return(value));
			EXPECT_CALL(*this, isDefault()).Times(AnyNumber()).WillRepeatedly(Return(false));
			EXPECT_CALL(*this, isNull()).Times(AnyNumber()).WillRepeatedly(Return(false));
		}

		MockFieldValue::~MockFieldValue()
		{
			if (m_mockField)
				delete m_mockField;
		}

		MOCK_CONST_METHOD0(getField, db::IField&());

		MOCK_CONST_METHOD0(isDefault, bool());
		MOCK_CONST_METHOD0(isNull, bool());
		MOCK_CONST_METHOD0(getBooleanValue, bool());
		MOCK_CONST_METHOD0(getIntValue, int());
		MOCK_CONST_METHOD0(getDoubleValue, double());
		MOCK_CONST_METHOD0(getStringValue, std::string());
		MOCK_CONST_METHOD0(getDateTimeValue, boost::posix_time::ptime());
		MOCK_CONST_METHOD0(getBinaryValue, db::IBinaryValue&());

		MOCK_METHOD1(setValue, void(const db::IFieldValue&));

		void MockFieldValue::setNull()
		{
			EXPECT_CALL(*this, isNull()).Times(AnyNumber()).WillRepeatedly(Return(true));
			EXPECT_CALL(*this, isDefault()).Times(AnyNumber()).WillRepeatedly(Return(false));
		}

		void MockFieldValue::setDefault()
		{
			EXPECT_CALL(*this, isDefault()).Times(AnyNumber()).WillRepeatedly(Return(true));
		}

		void MockFieldValue::setBooleanValue(bool value)
		{
			EXPECT_CALL(*this, getBooleanValue()).Times(AnyNumber()).WillRepeatedly(Return(value));
			EXPECT_CALL(*this, isNull()).Times(AnyNumber()).WillRepeatedly(Return(false));
			EXPECT_CALL(*this, isDefault()).Times(AnyNumber()).WillRepeatedly(Return(false));
		}

		void MockFieldValue::setIntValue(int value)
		{
			EXPECT_CALL(*this, getIntValue()).Times(AnyNumber()).WillRepeatedly(Return(value));
			EXPECT_CALL(*this, isNull()).Times(AnyNumber()).WillRepeatedly(Return(false));
			EXPECT_CALL(*this, isDefault()).Times(AnyNumber()).WillRepeatedly(Return(false));
		}

		void MockFieldValue::setDoubleValue(double value)
		{
			EXPECT_CALL(*this, getDoubleValue()).Times(AnyNumber()).WillRepeatedly(Return(value));
			EXPECT_CALL(*this, isNull()).Times(AnyNumber()).WillRepeatedly(Return(false));
			EXPECT_CALL(*this, isDefault()).Times(AnyNumber()).WillRepeatedly(Return(false));
		}

		MOCK_METHOD1(setStringValueProxy, void(const std::string&));
		void MockFieldValue::setStringValue(const std::string& value)
		{
			setStringValueProxy(value);
			EXPECT_CALL(*this, getStringValue()).Times(AnyNumber()).WillRepeatedly(Return(value));
			EXPECT_CALL(*this, isNull()).Times(AnyNumber()).WillRepeatedly(Return(false));
			EXPECT_CALL(*this, isDefault()).Times(AnyNumber()).WillRepeatedly(Return(false));
		}

		void MockFieldValue::setDateTimeValue(const boost::posix_time::ptime& value)
		{
			EXPECT_CALL(*this, getDateTimeValue()).Times(AnyNumber()).WillRepeatedly(Return(value));
			EXPECT_CALL(*this, isNull()).Times(AnyNumber()).WillRepeatedly(Return(false));
			EXPECT_CALL(*this, isDefault()).Times(AnyNumber()).WillRepeatedly(Return(false));
		}

		MOCK_METHOD1(setBinaryValueProxy, void(db::IBinaryValue*));
		void setBinaryValue(std::unique_ptr<db::IBinaryValue> value)
		{
			setBinaryValueProxy(value.release());
			EXPECT_CALL(*this, isNull()).Times(AnyNumber()).WillRepeatedly(Return(false));
			EXPECT_CALL(*this, isDefault()).Times(AnyNumber()).WillRepeatedly(Return(false));
		};

		MOCK_METHOD0(useDefaultValue, void());

		MOCK_CONST_METHOD0(cloneProxy, IFieldValue*());
		std::unique_ptr<IFieldValue> clone() const
		{
			return std::unique_ptr<IFieldValue>( cloneProxy() );
		}

		std::unique_ptr<MockFieldValue> cloneMock()
		{
			const db::IField& field = getField();
			std::string fieldName = field.getName();
			db::FieldTypes type = field.getType();

			switch (type)
			{
				case db::BOOLEAN:
					if (!isNull())
					{
						return std::unique_ptr<MockFieldValue>( new MockFieldValue(fieldName, getBooleanValue()) );
					}
					else
					{
						return std::unique_ptr<MockFieldValue>( new MockFieldValue(fieldName, boost::optional<bool>()) );
					}

				case db::INT:
					if (!isNull())
					{
						return std::unique_ptr<MockFieldValue>( new MockFieldValue(fieldName, getIntValue()) );
					}
					else
					{
						return std::unique_ptr<MockFieldValue>( new MockFieldValue(fieldName, boost::optional<int>()) );
					}

				case db::DOUBLE:
					if (!isNull())
					{
						return std::unique_ptr<MockFieldValue>( new MockFieldValue(fieldName, getDoubleValue()) );
					}
					else
					{
						return std::unique_ptr<MockFieldValue>( new MockFieldValue(fieldName, boost::optional<double>()) );
					}

				case db::STRING:
					if (!isNull())
					{
						return std::unique_ptr<MockFieldValue>( new MockFieldValue(fieldName, getStringValue()) );
					}
					else
					{
						return std::unique_ptr<MockFieldValue>( new MockFieldValue(fieldName, boost::optional<std::string>()) );
					}

				case db::DATETIME:
					return std::unique_ptr<MockFieldValue>( new MockFieldValue(fieldName, getDateTimeValue()) );

				default:
					throw std::exception( "Type not implemented in MockFieldValue" );
			}
		}

		friend bool operator== (const MockFieldValue& fieldValue1, const MockFieldValue& fieldValue2)
		{
			if (*fieldValue1.m_mockField != *fieldValue2.m_mockField)
			{
				return false;
			}

			if (fieldValue1.isDefault() != fieldValue2.isDefault())
			{
				return false;
			}

			if (fieldValue1.isNull() != fieldValue2.isNull())
			{
				return false;
			}

			if (!fieldValue1.isNull())
			{
				db::FieldTypes fieldType = fieldValue1.m_mockField->getType();
				switch (fieldType)
				{
					case db::BOOLEAN:
						return (fieldValue1.getBooleanValue() == fieldValue2.getBooleanValue());

					case db::INT:
						return (fieldValue1.getIntValue() == fieldValue2.getIntValue());

					case db::DOUBLE:
						return (fieldValue1.getDoubleValue() == fieldValue2.getDoubleValue());

					case db::STRING:
						return (fieldValue1.getStringValue() == fieldValue2.getStringValue());

					case db::DATETIME:
						return (fieldValue1.getDateTimeValue() == fieldValue2.getDateTimeValue());

					default:
						return false;
				}
			}
			else
			{
				return true;
			}
		}

		friend bool operator!= (const MockFieldValue& fieldValue1, const MockFieldValue& fieldValue2)
		{
			return !(fieldValue1 == fieldValue2);
		}

	private:
		MockField* m_mockField;
	};

}}

#endif //_DBADAPTERTESTUTILITIES_MOCKFIELDVALUE_QV_2201161915_H
#ifndef _DBADAPTERTESTUTILITIES_MOCKFIELD_QV_2202161907_H
#define _DBADAPTERTESTUTILITIES_MOCKFIELD_QV_2202161907_H

#include "DbAdapterInterface/IField.h"


namespace systelab { namespace test_utility {

	using namespace testing;

	class MockField : public db::IField
	{
	public:
		MockField()
		{
		}

		MockField(const std::string& name, db::FieldTypes type)
		{
			EXPECT_CALL(*this, getName()).Times(AnyNumber()).WillRepeatedly(Return(name));
			EXPECT_CALL(*this, getType()).Times(AnyNumber()).WillRepeatedly(Return(type));
		}

		MOCK_CONST_METHOD0(hasNullDefaultValue, bool());
		MOCK_CONST_METHOD0(getIndex, unsigned int());
		MOCK_CONST_METHOD0(getName, std::string());
		MOCK_CONST_METHOD0(getType, db::FieldTypes());
		MOCK_CONST_METHOD0(isPrimaryKey, bool());

		MOCK_CONST_METHOD0(getIntDefaultValue, int());
		MOCK_CONST_METHOD0(getBooleanDefaultValue, bool());
		MOCK_CONST_METHOD0(getDoubleDefaultValue, double());
		MOCK_CONST_METHOD0(getStringDefaultValue, std::string());
		MOCK_CONST_METHOD0(getDateTimeDefaultValue, boost::posix_time::ptime());
		MOCK_CONST_METHOD0(getBinaryDefaultValue, db::IBinaryValue&());

		friend bool operator== (const MockField& field1, const MockField& field2)
		{
			if (field1.getName() != field2.getName())
				return false;

			if (field1.getType() != field2.getType())
				return false;

			return true;
		}

		friend bool operator!= (const MockField& field1, const MockField& field2)
		{
			return !(field1 == field2);
		}

	};
}}

#endif //_DBADAPTERTESTUTILITIES_MOCKFIELD_QV_2202161907_H
#ifndef _DBADAPTERTESTUTILITIES_MOCKPRIMARYKEYVALUE_QV_1003161907_H
#define _DBADAPTERTESTUTILITIES_MOCKPRIMARYKEYVALUE_QV_1003161907_H

#include "DbAdapterInterface/IPrimaryKeyValue.h"


namespace systelab { namespace test_utility {

	using namespace testing;

	class MockPrimaryKeyValue : public db::IPrimaryKeyValue
	{
	public:
		MOCK_CONST_METHOD0(getTable, db::ITable&());
		MOCK_CONST_METHOD0(getPrimaryKey, db::IPrimaryKey&());

		MOCK_CONST_METHOD0(getFieldValuesCount, unsigned int());
		MOCK_CONST_METHOD1(getFieldValue, db::IFieldValue&(unsigned int));
		MOCK_CONST_METHOD1(getFieldValue, db::IFieldValue&(const std::string&));
	};
}}

#endif //_DBADAPTERTESTUTILITIES_MOCKPRIMARYKEYVALUE_QV_1003161907_H
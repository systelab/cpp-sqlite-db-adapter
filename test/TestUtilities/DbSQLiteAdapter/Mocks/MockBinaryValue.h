#ifndef _DBADAPTERTESTUTILITIES_MOCKBINARYVALUE_QV_2202161903_H
#define _DBADAPTERTESTUTILITIES_MOCKBINARYVALUE_QV_2202161903_H

#include "DbAdapterInterface/IBinaryValue.h"

namespace systelab { namespace test_utility {

	class MockBinaryValue : public IBinaryValue
	{
	public:
		MOCK_CONST_METHOD0(getOutputStream, std::ostream());
		MOCK_CONST_METHOD0(getInputStream, std::istream());
	};
}}

#endif //_DBADAPTERTESTUTILITIES_MOCKBINARYVALUE_QV_2202161903_H
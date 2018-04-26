#ifndef _DBADAPTERTESTUTILITIES_MOCKCONNECTIONCONFIGURATION_QV_2202161819_H
#define _DBADAPTERTESTUTILITIES_MOCKCONNECTIONCONFIGURATION_QV_2202161819_H

#include "DbAdapterInterface/IConnectionConfiguration.h"

namespace systelab { namespace test_utility {

	class MockConnectionConfiguration: public db::IConnectionConfiguration
	{
	public:
		MOCK_CONST_METHOD1(getParameter, std::string (const std::string& parameterName));
	};

}}

#endif //_DBADAPTERTESTUTILITIES_MOCKCONNECTIONCONFIGURATION_QV_2202161819_H
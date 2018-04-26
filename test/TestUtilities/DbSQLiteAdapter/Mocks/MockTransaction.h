#ifndef _DBADAPTERTESTUTILITIES_MOCKTRANSACTION_QV_2202161919_H
#define _DBADAPTERTESTUTILITIES_MOCKTRANSACTION_QV_2202161919_H

#include "DbAdapterInterface/ITransaction.h"

namespace systelab { namespace test_utility {

	class MockTransaction : public db::ITransaction
	{
	public:
		MOCK_METHOD0(commit, void());
		MOCK_METHOD0(rollback, void());
	};

}}

#endif //_DBADAPTERTESTUTILITIES_MOCKTRANSACTION_QV_2202161919_H
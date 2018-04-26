#ifndef _DBADAPTERTESTUTILITIES_MOCKCONNECTION_QV_2202161817_H
#define _DBADAPTERTESTUTILITIES_MOCKCONNECTION_QV_2202161817_H

#include "DbAdapterInterface/IConnection.h"

namespace systelab { namespace test_utility {

	class MockConnection : public db::IConnection
	{
	public:
		MOCK_METHOD1(loadDatabaseProxy, db::IDatabase* (db::IConnectionConfiguration&));
		std::unique_ptr<db::IDatabase> loadDatabase(db::IConnectionConfiguration& config)
		{
			return std::unique_ptr<db::IDatabase>(loadDatabaseProxy(config));
		}
	};

}}

#endif //_DBADAPTERTESTUTILITIES_MOCKCONNECTION_QV_2202161817_H
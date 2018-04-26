#ifndef _DBSQLITEADAPTER_CONNECTION_QUIM_VILA_2112151522_H
#define _DBSQLITEADAPTER_CONNECTION_QUIM_VILA_2112151522_H

#include "DbAdapterInterface/IConnection.h"


namespace systelab { namespace db { namespace sqlite {

	class Connection : public IConnection
	{
	public:
		Connection();
		virtual ~Connection();

		std::unique_ptr<IDatabase> loadDatabase(IConnectionConfiguration&);

	};

}}}

#endif //_DBSQLITEADAPTER_CONNECTION_QUIM_VILA_2112151522_H
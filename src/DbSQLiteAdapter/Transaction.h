#ifndef _DBSQLITEADAPTER_TRANSACTION_QUIM_VILA_2112151606_H
#define _DBSQLITEADAPTER_TRANSACTION_QUIM_VILA_2112151606_H

#include "DbAdapterInterface/ITransaction.h"

namespace systelab { namespace db {
	class IDatabase;
}}

namespace systelab { namespace db { namespace sqlite {

	class Transaction : public ITransaction
	{
	public:
		Transaction(IDatabase& database);
		virtual ~Transaction();

		void commit();
		void rollback();

	private:
		IDatabase& m_database;

	};

}}}

#endif //_DBSQLITEADAPTER_TRANSACTION_QUIM_VILA_2112151606_H
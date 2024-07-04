#pragma once

#include "DbAdapterInterface/ITransaction.h"

namespace systelab { namespace db {
	class IDatabase;
}}

namespace systelab { namespace db { namespace sqlite {

	class Transaction : public ITransaction
	{
	public:
		explicit Transaction(IDatabase& database);
		~Transaction();

		void commit() override;
		void rollback() override;

	private:
		IDatabase& m_database;
	};

}}}

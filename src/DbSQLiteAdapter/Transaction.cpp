#include "stdafx.h"
#include "Transaction.h"

#include "../DbAdapterInterface/IDatabase.h"


namespace systelab { namespace db { namespace sqlite {

	Transaction::Transaction(IDatabase& database)
		:m_database(database)
	{
	}

	Transaction::~Transaction()
	{
	}

	void Transaction::commit()
	{
		std::string operation = "COMMIT";
		m_database.executeOperation(operation);
	}

	void Transaction::rollback()
	{
		std::string operation = "ROLLBACK";
		m_database.executeOperation(operation);
	}

}}}
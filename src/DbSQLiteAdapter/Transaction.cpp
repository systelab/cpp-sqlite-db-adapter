#include "stdafx.h"
#include "Transaction.h"

#include "DbAdapterInterface/IDatabase.h"

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
		m_database.executeOperation("COMMIT");
	}

	void Transaction::rollback()
	{
		m_database.executeOperation("ROLLBACK");
	}

}}}
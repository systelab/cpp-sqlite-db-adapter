#ifndef _DBADAPTERTESTUTILITIES_MOCKDATABASE_QV_2202161822_H
#define _DBADAPTERTESTUTILITIES_MOCKDATABASE_QV_2202161822_H

#include "DbAdapterInterface/IDatabase.h"

namespace systelab { namespace test_utility {

	using namespace testing;

	class MockDatabase: public db::IDatabase
	{
	public:
		MockDatabase()
		{
		}

		~MockDatabase()
		{
			unsigned int nTables = m_tables.size();
			for (unsigned int i = 0; i < nTables; i++)
			{
				delete m_tables[i];
			}
		}

		MOCK_METHOD1(getTable, db::ITable& (std::string tableName));

		MOCK_METHOD1(executeQueryProxy, db::IRecordSet* (const std::string& query));
		std::unique_ptr<db::IRecordSet> executeQuery(const std::string& query)
		{
			return std::unique_ptr<db::IRecordSet>(executeQueryProxy(query));
		}

		MOCK_METHOD1(executeOperation, void (const std::string& operation));
		MOCK_METHOD1(executeMultipleStatements, void (const std::string& operation));
		MOCK_CONST_METHOD0(getRowsAffectedByLastChangeOperation, db::RowsAffected());
		MOCK_CONST_METHOD0(getLastInsertedRowId, db::RowId());

		MOCK_METHOD0(startTransactionProxy, db::ITransaction* ());
		std::unique_ptr<db::ITransaction> startTransaction()
		{
			return std::unique_ptr<db::ITransaction>(startTransactionProxy());
		};

		void addTable(std::unique_ptr<db::ITable> table)
		{
			db::ITable* tableRawPtr = table.release();
			m_tables.push_back( tableRawPtr );

			std::string tableName = tableRawPtr->getName();
			EXPECT_CALL(*this, getTable(tableName)).Times(AnyNumber()).WillRepeatedly(ReturnRef(*tableRawPtr));
		}

	private:
		std::vector<db::ITable*> m_tables;

	};

}}

#endif //_DBADAPTERTESTUTILITIES_MOCKDATABASE_QV_2202161822_H
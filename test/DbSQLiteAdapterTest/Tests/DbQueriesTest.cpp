#include "stdafx.h"

#include "DbSQLiteAdapter/Connection.h"
#include "DbSQLiteAdapter/ConnectionConfiguration.h"
#include "DbSQLiteAdapter/DateTimeHelper.h"

#include <DbAdapterInterface/IConnection.h>
#include <DbAdapterTestUtilities/Mocks/MockConnection.h>
#include <DbAdapterTestUtilities/Mocks/MockDatabase.h>
#include <DbAdapterTestUtilities/Mocks/MockTable.h>

namespace systelab { namespace db { namespace sqlite { namespace unit_test {

	static const std::string EVALUATION_TABLE_NAME = "TESTS";

	using namespace systelab::db;
	using namespace testing;

	class DbQueriesTest: public testing::Test
	{
	public:
		void SetUp()
		{
			ConnectionConfiguration configuration("database.db"s);
			m_db = m_connection.loadDatabase(configuration);
			dropTable(m_connection);
			createTable(m_connection);
			startTimeTracking();
		}

		void TearDown()
		{
			endTimeTracking();
			dropTable(m_connection);
		}

	protected:
		test_utility::MockTable m_table;
		test_utility::MockDatabase m_database;
		std::unique_ptr<IDatabase> m_db;
		systelab::db::sqlite::Connection m_connection;

		std::chrono::system_clock::time_point m_startTime;

		void startTimeTracking()
		{
			m_startTime = std::chrono::system_clock::now();
		}

		void endTimeTracking()
		{
			auto endTime = std::chrono::system_clock::now();
			std::cout << "\nExecution time: " << (endTime - m_startTime) << "\n";
		}

		void createTable(const systelab::db::sqlite::Connection& connection)
		{
			m_db->executeOperation("CREATE TABLE TESTS(ID INT PRIMARY KEY NOT NULL, FIELD_INT_INDEX INT, FIELD_INT_NO_INDEX INT, FIELD_STR_INDEX CHAR(255), FIELD_STR_NO_INDEX CHAR(255), FIELD_DATE DATETIME DEFAULT NULL)");
			m_db->executeOperation("CREATE INDEX INT_INDEX ON TESTS(FIELD_INT_INDEX)");
			m_db->executeOperation("CREATE INDEX STR_INDEX ON TESTS(FIELD_STR_INDEX)");

			std::unique_ptr<ITransaction> transaction = m_db->startTransaction();

			auto today = date_time::toDateTime("20150101T000000");
			for (unsigned int i = 0; i < 10000; i++)
			{
				std::ostringstream oss;
				std::string strDate = date_time::toISOString(today + std::chrono::days(7 * i));
				oss << "INSERT INTO TESTS (ID, FIELD_INT_INDEX, FIELD_INT_NO_INDEX, FIELD_STR_INDEX, FIELD_STR_NO_INDEX, FIELD_DATE) VALUES (" 
					<< i 
					<< ", " << i%7 
					<< ", " << i%10 
					<< ", \"STR" << i%9 << "\""
					<< ", \"STR" << i%12 << "\""
					<< ", '" << strDate << "'"
					<< ")";
				m_db->executeOperation(oss.str());
			}

			transaction->commit();
			transaction.reset();
		}

		void dropTable(const systelab::db::sqlite::Connection& connection)
		{
			m_db->executeOperation("DROP TABLE IF EXISTS TESTS");
		}

		unsigned int iterateTableRecordset(ITableRecordSet& recordSet)
		{
			unsigned int nRecords = 0;
			while (recordSet.isCurrentRecordValid())
			{
				const ITableRecord& record = recordSet.getCurrentRecord();
				++nRecords;
				recordSet.nextRecord();
			}

			return nRecords;
		}
		unsigned int iterateRecordset(IRecordSet& recordSet)
		{
			unsigned int nRecords = 0;
			while ( recordSet.isCurrentRecordValid() )
			{
				const IRecord& record = recordSet.getCurrentRecord();
				++nRecords;
				recordSet.nextRecord();
			}

			return nRecords;
		}
	};

	TEST_F(DbQueriesTest, testQueryAll)
	{
		ITable& table = m_db->getTable(EVALUATION_TABLE_NAME);
		std::unique_ptr<ITableRecordSet> recordSet = table.getAllRecords();
		ASSERT_EQ(10000, recordSet->getRecordsCount());
		iterateTableRecordset(*recordSet);
	}

	TEST_F(DbQueriesTest, testQueryRowPrimaryKey)
	{
		std::unique_ptr<IRecordSet> recordSet = m_db->executeQuery("SELECT * FROM TESTS WHERE ID = 50");
		ASSERT_EQ(1, recordSet->getRecordsCount());
		iterateRecordset(*recordSet);
	}

	TEST_F(DbQueriesTest, testQueryRowsNonIndexedIntField)
	{
		std::unique_ptr<IRecordSet> recordSet = m_db->executeQuery("SELECT * FROM TESTS WHERE FIELD_INT_NO_INDEX = 5");
		ASSERT_EQ(1000, recordSet->getRecordsCount());
		iterateRecordset(*recordSet);
	}

	TEST_F(DbQueriesTest, testQueryRowsIndexedIntField)
	{
		std::unique_ptr<IRecordSet> recordSet = m_db->executeQuery("SELECT * FROM TESTS WHERE FIELD_INT_INDEX = 5");
		ASSERT_EQ(1428, recordSet->getRecordsCount());
		iterateRecordset(*recordSet);
	}

	TEST_F(DbQueriesTest, testQueryRowsNonIndexedStringField)
	{
		std::unique_ptr<IRecordSet> recordSet = m_db->executeQuery("SELECT * FROM TESTS WHERE FIELD_STR_NO_INDEX = 'STR4'");
		ASSERT_EQ(833, recordSet->getRecordsCount());
		iterateRecordset(*recordSet);
	}

	TEST_F(DbQueriesTest, testQueryRowsIndexedStringField)
	{
		std::unique_ptr<IRecordSet> recordSet = m_db->executeQuery("SELECT * FROM TESTS WHERE FIELD_STR_INDEX = 'STR6'");
		ASSERT_EQ(1111, recordSet->getRecordsCount());
		iterateRecordset(*recordSet);
	}

	/*TEST_F(DbQueriesTest, testQueryRowsBetweenDates)
	{
		std::unique_ptr<IRecordSet> recordSet = m_db->executeQuery("SELECT * FROM TESTS WHERE FIELD_DATE BETWEEN '2015-06-01' AND '2016-01-01'");
		iterateRecordset(*recordSet);
	}*/

	TEST_F(DbQueriesTest, testSpace)
	{
	}

	TEST_F(DbQueriesTest, testInsertSingleRow)
	{
		m_db->executeOperation("INSERT INTO TESTS (ID, FIELD_INT_INDEX, FIELD_INT_NO_INDEX, FIELD_STR_INDEX, FIELD_STR_NO_INDEX) VALUES (10001, 5, 5, 'STR5', 'STR5')");
	}

	TEST_F(DbQueriesTest, testInsert1000RowsNoTransaction)
	{
		auto today = date_time::toDateTime("20180101T000000");
		for (unsigned int i = 10001; i < 11000; i++)
		{
			std::ostringstream oss;
			std::string strDate = date_time::toISOString(today + std::chrono::days(7 * i));
			oss << "INSERT INTO TESTS (ID, FIELD_INT_INDEX, FIELD_INT_NO_INDEX, FIELD_STR_INDEX, FIELD_STR_NO_INDEX) VALUES (" 
				<< i 
				<< ", " << i%7 
				<< ", " << i%10 
				<< ", 'STR" << i%9 << "'"
				<< ", 'STR" << i%12 << "'"
				<< ")";
			m_db->executeOperation(oss.str());
		}
	}

	TEST_F(DbQueriesTest, testInsert1000RowsTransaction)
	{
		std::unique_ptr<ITransaction> transaction = m_db->startTransaction();
		auto today = date_time::toDateTime("20180101T000000");
		for (unsigned int i = 10001; i < 11000; i++)
		{
			std::ostringstream oss;
			std::string strDate = date_time::toISOString(today + std::chrono::days(7 * i));
			oss << "INSERT INTO TESTS (ID, FIELD_INT_INDEX, FIELD_INT_NO_INDEX, FIELD_STR_INDEX, FIELD_STR_NO_INDEX) VALUES (" 
				<< i 
				<< ", " << i%7 
				<< ", " << i%10 
				<< ", 'STR" << i%9 << "'"
				<< ", 'STR" << i%12 << "'"
				<< ")";
			m_db->executeOperation(oss.str());
		}

		transaction->commit();
		transaction.reset();
	}

	TEST_F(DbQueriesTest, testUpdateIntFieldNotIndexed)
	{
		m_db->executeOperation("UPDATE TESTS SET FIELD_INT_NO_INDEX = 100 WHERE ID = 5");
	}

	TEST_F(DbQueriesTest, testUpdateIntFieldIndexed)
	{
		m_db->executeOperation("UPDATE TESTS SET FIELD_INT_INDEX = 100 WHERE ID = 5");
	}

	TEST_F(DbQueriesTest, testUpdateStringFieldNotIndexed1Row)
	{
		m_db->executeOperation("UPDATE TESTS SET FIELD_STR_NO_INDEX = 'STR100' WHERE ID = 5");
	}

	TEST_F(DbQueriesTest, testUpdateStringFieldIndexed1Row)
	{
		m_db->executeOperation("UPDATE TESTS SET FIELD_STR_INDEX = 'STR100' WHERE ID = 5");
	}

	TEST_F(DbQueriesTest, testUpdateStringFieldNotIndexed1000Row)
	{
		m_db->executeOperation("UPDATE TESTS SET FIELD_STR_NO_INDEX = 'STR100'");
	}

	TEST_F(DbQueriesTest, testUpdateStringFieldIndexed1000Row)
	{
		m_db->executeOperation("UPDATE TESTS SET FIELD_STR_INDEX = 'STR100'");
	}

	TEST_F(DbQueriesTest, testUpdateStringFieldNotIndexed1000RowDifferentValues)
	{
		for (unsigned int i = 0; i < 1000; i++)
		{
			std::ostringstream oss;
			oss << "UPDATE TESTS SET FIELD_STR_NO_INDEX = "
				<< "'STR" << (i + 1) << "'"
				<< " WHERE ID = " << i;
			m_db->executeOperation(oss.str());
		}
	}

	TEST_F(DbQueriesTest, testUpdateStringFieldIndexed1000RowDifferentValues)
	{
		std::unique_ptr<ITransaction> transaction = m_db->startTransaction();

		for (unsigned int i = 0; i < 1000; i++)
		{
			std::ostringstream oss;
			oss << "UPDATE TESTS SET FIELD_STR_INDEX = "
				<< "'STR" << (i + 1) << "'"
				<< " WHERE ID = " << i;
			m_db->executeOperation(oss.str());
		}

		transaction->commit();
		transaction.reset();
	}

	TEST_F(DbQueriesTest, testDeleteRecord)
	{
		m_db->executeOperation("DELETE FROM TESTS WHERE ID = 5");
	}

	TEST_F(DbQueriesTest, testDelete1000RecordsNoTransaction)
	{
		for (unsigned int i = 0; i < 1000; i++)
		{
			std::ostringstream oss;
			oss << "DELETE FROM TESTS WHERE ID = "
				<< i;
			m_db->executeOperation(oss.str());
		}
	}

	TEST_F(DbQueriesTest, testDelete1000RecordsTransaction)
	{
		std::unique_ptr<ITransaction> transaction = m_db->startTransaction();

		for (unsigned int i = 0; i < 1000; i++)
		{
			std::ostringstream oss;
			oss << "DELETE FROM TESTS WHERE ID = "
				<< i;
			m_db->executeOperation(oss.str());
		}

		transaction->commit();
		transaction.reset();
	}

}}}}


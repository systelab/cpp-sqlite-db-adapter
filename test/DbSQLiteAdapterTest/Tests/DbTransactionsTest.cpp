#include "stdafx.h"
#include "Helpers/Helpers.h"

#include "DbAdapterInterface/IConnection.h"
#include "DbSQLiteAdapter/Connection.h"
#include "DbSQLiteAdapter/ConnectionConfiguration.h"

#include <boost/filesystem.hpp>

using namespace testing;

namespace systelab { namespace db { namespace sqlite { namespace unit_test {

	static const std::string TRANSACTIONS_DATABASE_FILEPATH = "transactions-test.db";
	static const std::string MAIN_TABLE = "MAIN_TABLE";
	static const std::string DUMMY_TABLE = "DUMMY_TABLE";

	class DbTransactionsTest: public testing::Test
	{
	public:
		void SetUp()
		{
			if (boost::filesystem::exists(TRANSACTIONS_DATABASE_FILEPATH))
			{
				boost::filesystem::remove(TRANSACTIONS_DATABASE_FILEPATH);
			}

			systelab::db::sqlite::ConnectionConfiguration connectionConfiguration(TRANSACTIONS_DATABASE_FILEPATH, "keyForTest1234"s);
			m_db = m_connection.loadDatabase(connectionConfiguration);

			IDatabase& db = *(m_db.get());
			
			dropTable(db, DUMMY_TABLE);
			dropTable(db, MAIN_TABLE);
			m_db->executeOperation("CREATE TABLE "+DUMMY_TABLE+" (ID INT PRIMARY KEY NOT NULL, FIELD_INT INT, FIELD_STR CHAR(255), FIELD_DATE DATETIME DEFAULT '20150503T100045')");
		}

		void TearDown()
		{
			IDatabase& db = *(m_db.get());
			dropTable(db, DUMMY_TABLE);
			dropTable(db, MAIN_TABLE);
		}

	public:
		std::unique_ptr<IDatabase> m_db;
		systelab::db::sqlite::Connection m_connection;
	};
	
	// INTENDED USE 7: Transactions.

	TEST_F(DbTransactionsTest, testSQLOperationsInsertFieldsWithTransactionCommit)
	{
		std::unique_ptr<ITransaction> transaction = m_db->startTransaction();

		ITable& table = m_db->getTable(DUMMY_TABLE);
		
		for (unsigned int i = 0; i<4; i++)
		{
			// Create the record to insert
			std::unique_ptr<ITableRecord> record = table.createRecord();
			record->getFieldValue("ID").setIntValue(i+1);
			record->getFieldValue("FIELD_INT").setIntValue(2552);
			record->getFieldValue("FIELD_STR").setStringValue("hola");

			// Add the record to the table:
			table.insertRecord(*record.get());
		}
		transaction->commit();
		transaction.reset();

		// Check table contains inserted fields;
		std::unique_ptr<ITableRecordSet> recordSet = table.getAllRecords();
		

		ASSERT_EQ(recordSet->getRecordsCount(), 4);
	}	

	TEST_F(DbTransactionsTest, testSQLOperationsInsertFieldsWithTransactionRollback)
	{
		std::unique_ptr<ITransaction> transaction = m_db->startTransaction();

		ITable& table = m_db->getTable(DUMMY_TABLE);
		
		for (unsigned int i = 0; i<4; i++)
		{
			// Create the record to insert
			std::unique_ptr<ITableRecord> record = table.createRecord();
			record->getFieldValue("ID").setIntValue(i+1);
			record->getFieldValue("FIELD_INT").setIntValue(2552);
			record->getFieldValue("FIELD_STR").setStringValue("hola");

			// Add the record to the table:
			table.insertRecord(*record.get());
		}

		transaction->rollback();
		transaction.reset();

		// Check table contains inserted fields;
		std::unique_ptr<ITableRecordSet> recordSet = table.getAllRecords();

		ASSERT_EQ(recordSet->getRecordsCount(), 0);
	}

	// Delete with transaction commit
	TEST_F(DbTransactionsTest, testSQLOperationsDeleteRecordWithTransactionCommit)
	{
		IDatabase& db = *(m_db.get());

		createTable(db, MAIN_TABLE, 25);

		ITable& table = m_db->getTable(MAIN_TABLE);
		
		std::unique_ptr<ITransaction> transaction = db.startTransaction();

		// Now check there is no record on the table with ID = 3;
		std::unique_ptr<ITableRecordSet> recordSet = table.filterRecordsByCondition("ID = 3");
		
		std::unique_ptr<ITableRecord> record = table.createRecord();
		record->getFieldValue("ID").setIntValue(3);

		std::vector<IFieldValue*> fieldValues;
		fieldValues.push_back(&record->getFieldValue("ID"));

		int result = table.deleteRecordsByCondition(fieldValues);

		transaction->commit();
		transaction.reset();
		
		// Now check there is no record on the table with ID = 3;
		recordSet = table.filterRecordsByCondition("ID = 3");
		
		ASSERT_EQ(recordSet->getRecordsCount(), 0);
	}

	// Delete with transaction rollback
	TEST_F(DbTransactionsTest, testSQLOperationsDeleteRecordWithTransactionRollback)
	{
		IDatabase& db = *(m_db.get());

		createTable(db, MAIN_TABLE, 25);

		ITable& table = m_db->getTable(MAIN_TABLE);
		
		std::unique_ptr<ITransaction> transaction = db.startTransaction();

		// Now check there is no record on the table with ID = 3;
		std::unique_ptr<ITableRecordSet> recordSet = table.filterRecordsByCondition("ID = 3");
		
		std::unique_ptr<ITableRecord> record = table.createRecord();
		record->getFieldValue("ID").setIntValue(3);

		std::vector<IFieldValue*> fieldValues;
		fieldValues.push_back(&record->getFieldValue("ID"));

		int result = table.deleteRecordsByCondition(fieldValues);

		transaction->rollback();
		transaction.reset();

		// Now check there is no record on the table with ID = 3;
		recordSet = table.filterRecordsByCondition("ID = 3");
		
		ASSERT_EQ(recordSet->getRecordsCount(), 1);
	}
}}}}
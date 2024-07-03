#include "stdafx.h"
#include "Helpers/Helpers.h"

#include "DbSQLiteAdapter/Connection.h"
#include "DbSQLiteAdapter/ConnectionConfiguration.h"
#include "DbSQLiteAdapter/DateTimeHelper.h"

#include <DbAdapterInterface/IConnection.h>
#include <DbAdapterInterface/ITable.h>
#include <DbAdapterInterface/ITableRecord.h>
#include <filesystem>
#include <thread>


namespace systelab { namespace db { namespace sqlite { namespace unit_test {
	namespace
	{
		using namespace testing;
		using namespace std::chrono_literals;
	}

	static const std::string INSERT_DATABASE_FILEPATH = "insert-test.db";
	static const std::string INSERT_TABLE_NAME = "INSERT_TABLE";
	static const int INSERT_TABLE_NUM_RECORDS = 10;

	/**
	 * Tests if the insert operations over a table performed using the SQLite DB adapter
	 * work properly.
	 */
	class DbInsertOperationsTest: public testing::Test
	{
	public:
		void SetUp()
		{
			if (std::filesystem::exists(INSERT_DATABASE_FILEPATH))
			{
				std::filesystem::remove(INSERT_DATABASE_FILEPATH);
			}

			m_db = loadDatabase();
			dropTable(*m_db, INSERT_TABLE_NAME);
			createTable(*m_db, INSERT_TABLE_NAME, INSERT_TABLE_NUM_RECORDS);
		}

		std::unique_ptr<IDatabase> loadDatabase()
		{
			systelab::db::sqlite::ConnectionConfiguration connectionConfiguration(INSERT_DATABASE_FILEPATH, "keyForTest123"s);
			
			systelab::db::sqlite::Connection dbConnection;
			return dbConnection.loadDatabase(connectionConfiguration);
		}

		ITable& getInsertTable() const
		{
			return m_db->getTable(INSERT_TABLE_NAME);
		}

	private:
		std::string m_filepath;
		std::unique_ptr<IDatabase> m_db;
	};


	TEST_F(DbInsertOperationsTest, testNewRecordsAreInsertedSuccessfully)
	{
		// Insert new records into table
		ITable& table = getInsertTable();
		for (unsigned int i = INSERT_TABLE_NUM_RECORDS; i < INSERT_TABLE_NUM_RECORDS + 5; i++ )
		{
			std::unique_ptr<ITableRecord> record = table.createRecord();

			record->getFieldValue("ID").setIntValue(i);
			record->getFieldValue("FIELD_INT_INDEX").setIntValue(2552 + i);
			record->getFieldValue("FIELD_INT_NO_INDEX").setIntValue(140 + i);
			record->getFieldValue("FIELD_STR_INDEX").setStringValue(std::string("STR") + std::to_string((long long) i));
			record->getFieldValue("FIELD_STR_NO_INDEX").setStringValue(std::string("STR") + std::to_string((long long) (i*2)));
			record->getFieldValue("FIELD_REAL").setDoubleValue(1.2345 + i);
			record->getFieldValue("FIELD_BOOL").setBooleanValue((i%2) == 0);
			record->getFieldValue("FIELD_DATE").setDateTimeValue(date_time::toDateTime("20150203T000000") + std::chrono::days(i));

			RowsAffected nRows = table.insertRecord(*record.get());
			ASSERT_EQ(nRows, 1);
		}

		// Check new records have been stored successfully
		std::string conditionSQL = "ID >= " + std::to_string((long long) INSERT_TABLE_NUM_RECORDS);
		std::unique_ptr<ITableRecordSet> recordset = table.filterRecordsByCondition( conditionSQL );
		ASSERT_EQ(recordset->getRecordsCount(), 5);

		while(recordset->isCurrentRecordValid())
		{
			const ITableRecord& record = recordset->getCurrentRecord();

			int id = record.getFieldValue("ID").getIntValue();
			int expectedIntIndex = 2552 + id;
			int expectedIntNoIndex = 140 + id;
			std::string expectedStrIndex = "STR" + std::to_string((long long) id);
			std::string expectedStrNoIndex = "STR" + std::to_string((long long) (id*2));
			bool expectedBool = ((id%2) == 0);
			auto expectedDate = date_time::toDateTime("20150203T000000") + std::chrono::days(id);

			ASSERT_EQ(record.getFieldValue("FIELD_INT_INDEX").getIntValue(),		expectedIntIndex);
			ASSERT_EQ(record.getFieldValue("FIELD_INT_NO_INDEX").getIntValue(),		expectedIntNoIndex);
			ASSERT_EQ(record.getFieldValue("FIELD_STR_INDEX").getStringValue(),		expectedStrIndex);
			ASSERT_EQ(record.getFieldValue("FIELD_STR_NO_INDEX").getStringValue(),	expectedStrNoIndex);
			ASSERT_EQ(record.getFieldValue("FIELD_BOOL").getBooleanValue(),			expectedBool);
			ASSERT_EQ(record.getFieldValue("FIELD_DATE").getDateTimeValue(),		expectedDate);

			recordset->nextRecord();
		}
	}

	TEST_F(DbInsertOperationsTest, testInsertRecordThrowsAnExceptionIfRecordAlreadyExists)
	{
		// Create the record to insert
		ITable& table = getInsertTable();
		std::unique_ptr<ITableRecord> record = table.createRecord();
		record->getFieldValue("ID").setIntValue(25);
		record->getFieldValue("FIELD_INT_INDEX").setIntValue(1);
		record->getFieldValue("FIELD_INT_NO_INDEX").setIntValue(1);
		record->getFieldValue("FIELD_STR_INDEX").setStringValue("STR");
		record->getFieldValue("FIELD_STR_NO_INDEX").setStringValue("STR");
		record->getFieldValue("FIELD_REAL").setDoubleValue(1.0);
		record->getFieldValue("FIELD_BOOL").setBooleanValue(true);
		record->getFieldValue("FIELD_DATE").setDateTimeValue(DateTimeType());

		// Insert the record into table
		RowsAffected nRows = table.insertRecord(*record.get());
		ASSERT_EQ(nRows, 1);

		// Try to insert again the same record (without success)
		ASSERT_THROW(table.insertRecord(*record.get()), std::exception);
	}

	TEST_F(DbInsertOperationsTest, testInsertRecordFillsRecordWithGeneratedIdentifierAndDefaultValues)
	{
		// Create the record to insert
		ITable& table = getInsertTable();
		std::unique_ptr<ITableRecord> record = table.createRecord();
		record->getFieldValue("FIELD_INT_INDEX").setIntValue(1);
		record->getFieldValue("FIELD_STR_INDEX").setStringValue("STR");

		RowsAffected nRows = table.insertRecord(*record.get());
		ASSERT_EQ(1, nRows);

		// Check the identifier of the inserted record
		ASSERT_EQ(INSERT_TABLE_NUM_RECORDS + 1, record->getFieldValue("ID").getIntValue());

		// Check that values of the inserted record are the default values
		ASSERT_EQ(2, record->getFieldValue("FIELD_INT_NO_INDEX").getIntValue());
		ASSERT_EQ("FIELD_STR_NO_INDEX", record->getFieldValue("FIELD_STR_NO_INDEX").getStringValue());
		ASSERT_NEAR(3.3, record->getFieldValue("FIELD_REAL").getDoubleValue(), 1e-10);
		ASSERT_EQ(false, record->getFieldValue("FIELD_BOOL").getBooleanValue());

		auto expectedDate = date_time::toDateTime("20160102T000000") + 3h + 4min + 5s;
		ASSERT_EQ(expectedDate, record->getFieldValue("FIELD_DATE").getDateTimeValue());
	}


	/**
	* Tests if conncurrent insert operations over different tables performed using the SQLite DB adapter
	* work properly.
	*/
	static const std::string INSERT_TABLE1_NAME = "INSERT_TABLE1";
	static const std::string INSERT_TABLE2_NAME = "INSERT_TABLE2";
	static const int INSERT_TABLE1_NUM_RECORDS = 0;
	static const int INSERT_TABLE2_NUM_RECORDS = 100;

	class DbInsertOperationsConcurrentTest: public testing::Test
	{
	public:
		void SetUp()
		{
			m_db = loadDatabase();
			dropTable(*m_db, INSERT_TABLE1_NAME);
			dropTable(*m_db, INSERT_TABLE2_NAME);
			createTable(*m_db, INSERT_TABLE1_NAME, INSERT_TABLE1_NUM_RECORDS);
			createTable(*m_db, INSERT_TABLE2_NAME, INSERT_TABLE2_NUM_RECORDS);
		}

		void TearDown()
		{
			dropTable(*m_db, INSERT_TABLE1_NAME);
			dropTable(*m_db, INSERT_TABLE2_NAME);
		}

		std::unique_ptr<IDatabase> loadDatabase()
		{
			systelab::db::sqlite::ConnectionConfiguration connectionConfiguration("sqlite-test.db"s, "keyForTest1234"s);

			systelab::db::sqlite::Connection dbConnection;
			return dbConnection.loadDatabase(connectionConfiguration);
		}

		ITable& getInsertTable1() const
		{
			return m_db->getTable(INSERT_TABLE1_NAME);
		}

		ITable& getInsertTable2() const
		{
			return m_db->getTable(INSERT_TABLE2_NAME);
		}

	private:
		std::unique_ptr<IDatabase> m_db;
	};


	TEST_F(DbInsertOperationsConcurrentTest, testConcurrentInsertsOnDifferentTablesDoNotCollide)
	{
		ITable& table1 = getInsertTable1();
		ITable& table2 = getInsertTable2();
		unsigned int nInserts = 250;

		std::thread insertsOnTable1Thread(
			[&table1, nInserts]()
			{
				for (unsigned int i = 0; i < nInserts; i++)
				{
					std::unique_ptr<ITableRecord> newRecord = table1.createRecord();
					newRecord->getFieldValue("FIELD_INT_INDEX").setIntValue(101 + i);
					newRecord->getFieldValue("FIELD_STR_INDEX").setStringValue("NEW-RECORD-TABLE1-" + std::to_string(i));

					RowsAffected nRows = table1.insertRecord(*newRecord.get());

					unsigned int expectedRecordId = INSERT_TABLE1_NUM_RECORDS + 1 + i;
					EXPECT_EQ(expectedRecordId, newRecord->getFieldValue("ID").getIntValue()) << " for insertion #" << i;
				}
			}
		);

		std::thread insertsOnTable2Thread(
			[&table2, nInserts]()
			{
				for (unsigned int i = 0; i < nInserts; i++)
				{
					std::unique_ptr<ITableRecord> newRecord = table2.createRecord();
					newRecord->getFieldValue("FIELD_INT_INDEX").setIntValue(202 + i);
					newRecord->getFieldValue("FIELD_STR_INDEX").setStringValue("NEW-RECORD-TABLE2-" + std::to_string(i));

					RowsAffected nRows = table2.insertRecord(*newRecord.get());

					unsigned int expectedRecordId = INSERT_TABLE2_NUM_RECORDS + 1 + i;
					EXPECT_EQ(expectedRecordId, newRecord->getFieldValue("ID").getIntValue()) << " for insertion #" << i;
				}
			}
		);

		insertsOnTable1Thread.join();
		insertsOnTable2Thread.join();
	}

}}}}

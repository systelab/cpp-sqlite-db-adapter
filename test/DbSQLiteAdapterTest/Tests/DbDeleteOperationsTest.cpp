#include "stdafx.h"
#include "Helpers/Helpers.h"

#include "DbAdapterInterface/IConnection.h"
#include "DbSQLiteAdapter/Connection.h"
#include "DbSQLiteAdapter/ConnectionConfiguration.h"

#include <boost/filesystem.hpp>

using namespace testing;

namespace systelab { namespace db { namespace sqlite { namespace unit_test {

	static const std::string DELETE_DATABASE_FILEPATH = "delete-keys-test.db";
	static const std::string DELETE_TABLE_NAME = "DELETE_TABLE";
	static const int DELETE_TABLE_NUM_RECORDS = 50;

	/**
	 * Tests if the delete operations over a table performed using the SQLite DB adapter
	 * work properly.
	 */
	class DbDeleteOperationsTest: public testing::Test
	{
	public:
		void SetUp()
		{
			if (boost::filesystem::exists(DELETE_DATABASE_FILEPATH))
			{
				boost::filesystem::remove(DELETE_DATABASE_FILEPATH);
			}

			m_db = loadDatabase();
			dropTable(*m_db, DELETE_TABLE_NAME);
			createTable(*m_db, DELETE_TABLE_NAME, DELETE_TABLE_NUM_RECORDS);
		}

		std::unique_ptr<IDatabase> loadDatabase()
		{
			systelab::db::sqlite::ConnectionConfiguration connectionConfiguration(DELETE_DATABASE_FILEPATH, "keyForTest1234"s);

			systelab::db::sqlite::Connection dbConnection;
			return dbConnection.loadDatabase(connectionConfiguration);
		}

		ITable& getDeleteTable() const
		{
			return m_db->getTable(DELETE_TABLE_NAME);
		}

	private:
		std::unique_ptr<IDatabase> m_db;
	};


	TEST_F(DbDeleteOperationsTest, testDeleteSingleRecord)
	{
		ITable& table = getDeleteTable();
		std::unique_ptr<IPrimaryKeyValue> primaryKeyValue = table.createPrimaryKeyValue();
		primaryKeyValue->getFieldValue("ID").setIntValue(14);

		// Delete the single record
		RowsAffected nRows = table.deleteRecord(*primaryKeyValue);
		ASSERT_EQ(nRows, 1);

		// Check that the record has been deleted successfully
		std::unique_ptr<ITableRecord> nullRecord = table.getRecordByPrimaryKey(*primaryKeyValue);
		ASSERT_TRUE(nullRecord.get() == 0);
	}

	TEST_F(DbDeleteOperationsTest, testDeleteNonExistingRecordAffectsZeroRows)
	{
		ITable& table = getDeleteTable();
		std::unique_ptr<ITableRecord> nonExistingRecord = table.createRecord();
		nonExistingRecord->getFieldValue("ID").setIntValue(-1);

		RowsAffected nRows = table.deleteRecord(*nonExistingRecord);
		ASSERT_EQ(nRows, 0);
	}

	TEST_F(DbDeleteOperationsTest, testDeleteMultipleRecordsByCondition)
	{
		ITable& table = getDeleteTable();
		std::unique_ptr<IFieldValue> strIndexValue = table.createFieldValue( table.getField("FIELD_STR_INDEX"), std::string("STR0") );

		std::vector<IFieldValue*> conditionValues;
		conditionValues.push_back( &(*strIndexValue) );

		int expectedAffectedRows = (int) ceil (DELETE_TABLE_NUM_RECORDS / 9.);
		RowsAffected nRows = table.deleteRecordsByCondition(conditionValues);
		ASSERT_EQ(nRows, expectedAffectedRows);

		std::unique_ptr<ITableRecordSet> recordset = table.filterRecordsByFields(conditionValues);
		ASSERT_EQ(recordset->getRecordsCount(), 0);
	}

}}}}

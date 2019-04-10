#include "stdafx.h"
#include "Helpers/Helpers.h"

#include "DbAdapterInterface/IConnection.h"
#include "DbSQLiteAdapter/Connection.h"
#include "DbSQLiteAdapter/ConnectionConfiguration.h"

#include <boost/filesystem.hpp>
#include <string>

using namespace testing;

namespace systelab { namespace db { namespace sqlite { namespace unit_test {

	static const std::string QUERY_DATABASE_FILEPATH = "query-test.db";
	static const std::string QUERY_TABLE_NAME = "QUERY_TABLE";
	static const int QUERY_TABLE_NUM_RECORDS = 100;

	/**
	 * Tests if the query operations over a table performed using the SQLite DB adapter
	 * return the data stored on that table.
	 */
	class DbQueryOperationsTest : public testing::Test
	{
	public:
		void SetUp()
		{
			if (boost::filesystem::exists(QUERY_DATABASE_FILEPATH))
			{
				boost::filesystem::remove(QUERY_DATABASE_FILEPATH);
			}

			m_db = loadDatabase();
			dropTable(*m_db, QUERY_TABLE_NAME);
			createTable(*m_db, QUERY_TABLE_NAME, QUERY_TABLE_NUM_RECORDS);
		}

		std::unique_ptr<IDatabase> loadDatabase()
		{
			systelab::db::sqlite::ConnectionConfiguration connectionConfiguration("sqlite-test.db"s, "keyForTest1234"s);
			systelab::db::sqlite::Connection dbConnection;
			return dbConnection.loadDatabase(connectionConfiguration);
		}

		ITable& getQueryTable() const
		{
			return m_db->getTable(QUERY_TABLE_NAME);
		}

		void assertRecordSet(ITableRecordSet& recordset)
		{
			while (recordset.isCurrentRecordValid())
			{
				const ITableRecord& record = recordset.getCurrentRecord();
				assertRecord(record);
				recordset.nextRecord();
			}
		}

		void assertRecord(const ITableRecord& record)
		{
			int id = record.getFieldValue("ID").getIntValue();
			int fieldIntIndex = record.getFieldValue("FIELD_INT_INDEX").getIntValue();
			int fieldIntNoIndex = record.getFieldValue("FIELD_INT_NO_INDEX").getIntValue();
			std::string fieldStrIndex = record.getFieldValue("FIELD_STR_INDEX").getStringValue();
			std::string fieldStrNoIndex = record.getFieldValue("FIELD_STR_NO_INDEX").getStringValue();
			double fieldReal = record.getFieldValue("FIELD_REAL").getDoubleValue();
			bool fieldBool = record.getFieldValue("FIELD_BOOL").getBooleanValue();
			boost::posix_time::ptime fieldDateTime = record.getFieldValue("FIELD_DATE").getDateTimeValue();

			ASSERT_EQ(fieldIntIndex,	getFieldIntIndexValue(id));
			ASSERT_EQ(fieldIntNoIndex,	getFieldIntNoIndexValue(id));
			ASSERT_EQ(fieldStrIndex,	getFieldStringIndexValue(id));
			ASSERT_EQ(fieldStrNoIndex,	getFieldStringNoIndexValue(id));
			ASSERT_EQ(fieldReal,		getFieldRealValue(id));
			ASSERT_EQ(fieldBool,		getFieldBooleanValue(id));
			ASSERT_EQ(fieldDateTime,	getFieldDateValue(id));
		}

	private:
		std::unique_ptr<IDatabase> m_db;
	};
	

	TEST_F(DbQueryOperationsTest, testQueryAll)
	{
		std::unique_ptr<ITableRecordSet> recordset = getQueryTable().getAllRecords();
		ASSERT_EQ(QUERY_TABLE_NUM_RECORDS, recordset->getRecordsCount());
		assertRecordSet(*recordset);
	}

	TEST_F(DbQueryOperationsTest, testQueryByPrimaryKey)
	{
		ITable& table = getQueryTable();
		std::unique_ptr<IPrimaryKeyValue> primaryKeyValue = table.createPrimaryKeyValue();
		primaryKeyValue->getFieldValue( "ID" ).setIntValue(27);

		std::unique_ptr<ITableRecord> record = table.getRecordByPrimaryKey(*primaryKeyValue);
		ASSERT_TRUE(record.get() != 0);
		ASSERT_EQ(record->getFieldValue("ID").getIntValue(), 27);
		assertRecord(*record);
	}

	TEST_F(DbQueryOperationsTest, testQueryWhenFieldIntIndexIsZero)
	{
		ITable& table = getQueryTable();
		const IField& fieldIntIndex = table.getField("FIELD_INT_INDEX");
		std::unique_ptr<IFieldValue> fieldIntIndexValue = table.createFieldValue(fieldIntIndex, 0);

		unsigned int expectedRecords = getNumRecordsWithFieldIntIndexZero(QUERY_TABLE_NUM_RECORDS);
		std::unique_ptr<ITableRecordSet> recordset = table.filterRecordsByField(*fieldIntIndexValue);
		ASSERT_EQ(expectedRecords, recordset->getRecordsCount());

		while (recordset->isCurrentRecordValid())
		{
			const ITableRecord& record = recordset->getCurrentRecord();
			ASSERT_EQ(record.getFieldValue("FIELD_INT_INDEX").getIntValue(), 0);
			assertRecord(record);
			recordset->nextRecord();
		}
	}

	TEST_F(DbQueryOperationsTest, testQueryWhenFieldIntNoIndexIsZero)
	{
		ITable& table = getQueryTable();
		const IField& fieldIntNoIndex = table.getField("FIELD_INT_NO_INDEX");
		std::unique_ptr<IFieldValue> fieldIntNoIndexValue = table.createFieldValue(fieldIntNoIndex, 0);

		unsigned int expectedRecords = getNumRecordsWithFieldIntNoIndexZero(QUERY_TABLE_NUM_RECORDS);
		std::unique_ptr<ITableRecordSet> recordset = table.filterRecordsByField(*fieldIntNoIndexValue);
		ASSERT_EQ(expectedRecords, recordset->getRecordsCount());

		while (recordset->isCurrentRecordValid())
		{
			const ITableRecord& record = recordset->getCurrentRecord();
			ASSERT_EQ(record.getFieldValue("FIELD_INT_NO_INDEX").getIntValue(), 0);
			assertRecord(record);
			recordset->nextRecord();
		}
	}

	TEST_F(DbQueryOperationsTest, testQueryWhenFieldStrIndexIsSTR0)
	{
		ITable& table = getQueryTable();
		const IField& fieldStrIndex = table.getField("FIELD_STR_INDEX");
		std::unique_ptr<IFieldValue> fieldStrIndexValue = table.createFieldValue(fieldStrIndex, std::string("STR0"));

		unsigned int expectedRecords = getNumRecordsWithFieldStringIndexZero(QUERY_TABLE_NUM_RECORDS);
		std::unique_ptr<ITableRecordSet> recordset = table.filterRecordsByField(*fieldStrIndexValue);
		ASSERT_EQ(expectedRecords, recordset->getRecordsCount());

		while (recordset->isCurrentRecordValid())
		{
			const ITableRecord& record = recordset->getCurrentRecord();
			ASSERT_EQ(record.getFieldValue("FIELD_STR_INDEX").getStringValue(), std::string("STR0"));
			assertRecord(record);
			recordset->nextRecord();
		}
	}

	TEST_F(DbQueryOperationsTest, testQueryWhenFieldStrNoIndexIsSTR0)
	{
		ITable& table = getQueryTable();
		const IField& fieldStrNoIndex = table.getField("FIELD_STR_NO_INDEX");
		std::unique_ptr<IFieldValue> fieldStrNoIndexValue = table.createFieldValue(fieldStrNoIndex, std::string("STR0"));

		int expectedRecords = getNumRecordsWithFieldStringNoIndexZero(QUERY_TABLE_NUM_RECORDS);
		std::unique_ptr<ITableRecordSet> recordset = table.filterRecordsByField(*fieldStrNoIndexValue);
		ASSERT_EQ(expectedRecords, recordset->getRecordsCount());

		while (recordset->isCurrentRecordValid())
		{
			const ITableRecord& record = recordset->getCurrentRecord();
			ASSERT_EQ(record.getFieldValue("FIELD_STR_NO_INDEX").getStringValue(), std::string("STR0"));
			assertRecord(record);
			recordset->nextRecord();
		}
	}

	TEST_F(DbQueryOperationsTest, testQueryWhenFieldRealIsZero)
	{
		ITable& table = getQueryTable();
		const IField& fieldReal = table.getField("FIELD_REAL");
		std::unique_ptr<IFieldValue> fieldRealValue = table.createFieldValue(fieldReal, 0.);

		int expectedRecords = getNumRecordsWithFieldRealZero(QUERY_TABLE_NUM_RECORDS);
		std::unique_ptr<ITableRecordSet> recordset = table.filterRecordsByField(*fieldRealValue);
		ASSERT_EQ(expectedRecords, recordset->getRecordsCount());

		while (recordset->isCurrentRecordValid())
		{
			const ITableRecord& record = recordset->getCurrentRecord();
			ASSERT_EQ(record.getFieldValue("FIELD_REAL").getDoubleValue(), 0.);
			assertRecord(record);
			recordset->nextRecord();
		}
	}

	TEST_F(DbQueryOperationsTest, testQueryWhenFieldBoolIsTrue)
	{
		ITable& table = getQueryTable();
		const IField& fieldBool = table.getField("FIELD_BOOL");
		std::unique_ptr<IFieldValue> fieldBoolValue = table.createFieldValue(fieldBool, true);

		int expectedRecords = getNumRecordsWithFieldBoolTrue(QUERY_TABLE_NUM_RECORDS);
		std::unique_ptr<ITableRecordSet> recordset = table.filterRecordsByField(*fieldBoolValue);
		ASSERT_EQ(expectedRecords, recordset->getRecordsCount());

		while (recordset->isCurrentRecordValid())
		{
			const ITableRecord& record = recordset->getCurrentRecord();
			ASSERT_EQ(record.getFieldValue("FIELD_BOOL").getBooleanValue(), true);
			assertRecord(record);
			recordset->nextRecord();
		}
	}

	TEST_F(DbQueryOperationsTest, testQueryWhenFieldDateIsBaseDate)
	{
		boost::posix_time::ptime baseDate(boost::gregorian::date(2015,1,1));

		ITable& table = getQueryTable();
		const IField& fieldDate = table.getField("FIELD_DATE");
		std::unique_ptr<IFieldValue> fieldDateValue = table.createFieldValue(fieldDate, baseDate);

		int expectedRecords = getNumRecordsWithFieldDateIsBaseDate(QUERY_TABLE_NUM_RECORDS);
		std::unique_ptr<ITableRecordSet> recordset = table.filterRecordsByField(*fieldDateValue);
		ASSERT_EQ(expectedRecords, recordset->getRecordsCount());

		while (recordset->isCurrentRecordValid())
		{
			const ITableRecord& record = recordset->getCurrentRecord();
			ASSERT_EQ(record.getFieldValue("FIELD_DATE").getDateTimeValue(), baseDate);
			assertRecord(record);
			recordset->nextRecord();
		}
	}

}}}}
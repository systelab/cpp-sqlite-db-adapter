////#include "stdafx.h"
//#include "Utilities.h"

//#include "Connection.h"
//#include "cpp-db-adapter/IConnection.h"

//#include
//"../TestUtilities/DbSQLiteAdapter/Mocks/MockConnectionConfiguration.h"

// namespace db {
// namespace unit_test {

// using namespace systelab::db;
// using namespace testing;
// using namespace utilities;

// static const std::string INSERT_TABLE_NAME = "INSERT_TABLE";
// static const int INSERT_TABLE_NUM_RECORDS = 10;

///**
// * Tests if the insert operations over a table performed using the SQLite DB
// * adapter work properly.
// */
// class DbInsertOperationsTest : public testing::Test {
// public:
//  void SetUp() {
//    m_db = loadDatabase();
//    dropTable(*m_db, INSERT_TABLE_NAME);
//    createTable(*m_db, INSERT_TABLE_NAME, INSERT_TABLE_NUM_RECORDS);
//  }

//  void TearDown() { dropTable(*m_db, INSERT_TABLE_NAME); }

//  std::unique_ptr<IDatabase> loadDatabase() {
//    systelab::test_utility::MockConnectionConfiguration
//    connectionConfiguration; EXPECT_CALL(connectionConfiguration,
//    getParameter("filepath"))
//        .WillRepeatedly(Return("sqlite-test.db"));

//    systelab::db::sqlite::Connection dbConnection;
//    return dbConnection.loadDatabase(connectionConfiguration);
//  }

//  ITable &getInsertTable() const { return m_db->getTable(INSERT_TABLE_NAME); }

// private:
//  std::unique_ptr<IDatabase> m_db;
//};

// TEST_F(DbInsertOperationsTest, testNewRecordsAreInsertedSuccessfully) {
//  // Insert new records into table
//  ITable &table = getInsertTable();
//  for (unsigned int i = INSERT_TABLE_NUM_RECORDS;
//       i < INSERT_TABLE_NUM_RECORDS + 5; i++) {
//    std::unique_ptr<ITableRecord> record = table.createRecord();

//    record->getFieldValue("ID").setIntValue(i);
//    record->getFieldValue("FIELD_INT_INDEX").setIntValue(2552 + i);
//    record->getFieldValue("FIELD_INT_NO_INDEX").setIntValue(140 + i);
//    record->getFieldValue("FIELD_STR_INDEX")
//        .setStringValue(std::string("STR") + std::to_string((long long)i));
//    record->getFieldValue("FIELD_STR_NO_INDEX")
//        .setStringValue(std::string("STR") +
//                        std::to_string((long long)(i * 2)));
//    record->getFieldValue("FIELD_REAL").setDoubleValue(1.2345 + i);
//    record->getFieldValue("FIELD_BOOL").setBooleanValue((i % 2) == 0);
//    record->getFieldValue("FIELD_DATE")
//        .setDateTimeValue(boost::posix_time::ptime(
//            boost::gregorian::date(2015, 2, 3) + boost::gregorian::days(i)));

//    RowsAffected nRows = table.insertRecord(*record.get());
//    ASSERT_EQ(nRows, 1);
//  }

//  // Check new records have been stored successfully
//  std::string conditionSQL =
//      "ID >= " + std::to_string((long long)INSERT_TABLE_NUM_RECORDS);
//  std::unique_ptr<ITableRecordSet> recordset =
//      table.filterRecordsByCondition(conditionSQL);
//  ASSERT_EQ(recordset->getRecordsCount(), 5);

//  while (recordset->isCurrentRecordValid()) {
//    const ITableRecord &record = recordset->getCurrentRecord();

//    int id = record.getFieldValue("ID").getIntValue();
//    int expectedIntIndex = 2552 + id;
//    int expectedIntNoIndex = 140 + id;
//    std::string expectedStrIndex = "STR" + std::to_string((long long)id);
//    std::string expectedStrNoIndex =
//        "STR" + std::to_string((long long)(id * 2));
//    bool expectedBool = ((id % 2) == 0);
//    boost::posix_time::ptime expectedDate(boost::gregorian::date(2015, 2, 3) +
//                                          boost::gregorian::days(id));

//    ASSERT_EQ(record.getFieldValue("FIELD_INT_INDEX").getIntValue(),
//              expectedIntIndex);
//    ASSERT_EQ(record.getFieldValue("FIELD_INT_NO_INDEX").getIntValue(),
//              expectedIntNoIndex);
//    ASSERT_EQ(record.getFieldValue("FIELD_STR_INDEX").getStringValue(),
//              expectedStrIndex);
//    ASSERT_EQ(record.getFieldValue("FIELD_STR_NO_INDEX").getStringValue(),
//              expectedStrNoIndex);
//    ASSERT_EQ(record.getFieldValue("FIELD_BOOL").getBooleanValue(),
//              expectedBool);
//    ASSERT_EQ(record.getFieldValue("FIELD_DATE").getDateTimeValue(),
//              expectedDate);

//    recordset->nextRecord();
//  }
//}

// TEST_F(DbInsertOperationsTest,
//       testInsertRecordThrowsAnExceptionIfRecordAlreadyExists) {
//  // Create the record to insert
//  ITable &table = getInsertTable();
//  std::unique_ptr<ITableRecord> record = table.createRecord();
//  record->getFieldValue("ID").setIntValue(25);
//  record->getFieldValue("FIELD_INT_INDEX").setIntValue(1);
//  record->getFieldValue("FIELD_INT_NO_INDEX").setIntValue(1);
//  record->getFieldValue("FIELD_STR_INDEX").setStringValue("STR");
//  record->getFieldValue("FIELD_STR_NO_INDEX").setStringValue("STR");
//  record->getFieldValue("FIELD_REAL").setDoubleValue(1.0);
//  record->getFieldValue("FIELD_BOOL").setBooleanValue(true);
//  record->getFieldValue("FIELD_DATE")
//      .setDateTimeValue(boost::posix_time::ptime());

//  // Insert the record into table
//  RowsAffected nRows = table.insertRecord(*record.get());
//  ASSERT_EQ(nRows, 1);

//  // Try to insert again the same record (without success)
//  ASSERT_THROW(table.insertRecord(*record.get()), std::exception);
//}

// TEST_F(DbInsertOperationsTest,
//       testInsertRecordFillsRecordWithGeneratedIdentifierAndDefaultValues) {
//  // Create the record to insert
//  ITable &table = getInsertTable();
//  std::unique_ptr<ITableRecord> record = table.createRecord();
//  record->getFieldValue("FIELD_INT_INDEX").setIntValue(1);
//  record->getFieldValue("FIELD_STR_INDEX").setStringValue("STR");

//  RowsAffected nRows = table.insertRecord(*record.get());
//  ASSERT_EQ(1, nRows);

//  // Check the identifier of the inserted record
//  ASSERT_EQ(INSERT_TABLE_NUM_RECORDS + 1,
//            record->getFieldValue("ID").getIntValue());

//  // Check that values of the inserted record are the default values
//  ASSERT_EQ(2, record->getFieldValue("FIELD_INT_NO_INDEX").getIntValue());
//  ASSERT_EQ("FIELD_STR_NO_INDEX",
//            record->getFieldValue("FIELD_STR_NO_INDEX").getStringValue());
//  ASSERT_NEAR(3.3, record->getFieldValue("FIELD_REAL").getDoubleValue(),
//  1e-10); ASSERT_EQ(false,
//  record->getFieldValue("FIELD_BOOL").getBooleanValue());

//  boost::posix_time::ptime expectedDate(
//      boost::gregorian::date(2016, 1, 2)); // 20160102T030405
//  expectedDate += boost::posix_time::hours(3);
//  expectedDate += boost::posix_time::minutes(4);
//  expectedDate += boost::posix_time::seconds(5);
//  ASSERT_EQ(expectedDate,
//            record->getFieldValue("FIELD_DATE").getDateTimeValue());
//}

//} // namespace unit_test
//} // namespace db

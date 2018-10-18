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

// static const std::string DELETE_TABLE_NAME = "DELETE_TABLE";
// static const int DELETE_TABLE_NUM_RECORDS = 50;

///**
// * Tests if the delete operations over a table performed using the SQLite DB
// * adapter work properly.
// */
// class DbDeleteOperationsTest : public testing::Test {
// public:
//  void SetUp() {
//    m_db = loadDatabase();
//    dropTable(*m_db, DELETE_TABLE_NAME);
//    createTable(*m_db, DELETE_TABLE_NAME, DELETE_TABLE_NUM_RECORDS);
//  }

//  void TearDown() { dropTable(*m_db, DELETE_TABLE_NAME); }

//  std::unique_ptr<IDatabase> loadDatabase() {
//    systelab::test_utility::MockConnectionConfiguration
//    connectionConfiguration; EXPECT_CALL(connectionConfiguration,
//    getParameter("filepath"))
//        .WillRepeatedly(Return("sqlite-test.db"));

//    systelab::db::sqlite::Connection dbConnection;
//    return dbConnection.loadDatabase(connectionConfiguration);
//  }

//  ITable &getDeleteTable() const { return m_db->getTable(DELETE_TABLE_NAME); }

// private:
//  std::unique_ptr<IDatabase> m_db;
//};

// TEST_F(DbDeleteOperationsTest, testDeleteSingleRecord) {
//  ITable &table = getDeleteTable();
//  std::unique_ptr<IPrimaryKeyValue> primaryKeyValue =
//      table.createPrimaryKeyValue();
//  primaryKeyValue->getFieldValue("ID").setIntValue(14);

//  // Delete the single record
//  RowsAffected nRows = table.deleteRecord(*primaryKeyValue);
//  ASSERT_EQ(nRows, 1);

//  // Check that the record has been deleted successfully
//  std::unique_ptr<ITableRecord> nullRecord =
//      table.getRecordByPrimaryKey(*primaryKeyValue);
//  ASSERT_TRUE(nullRecord.get() == 0);
//}

// TEST_F(DbDeleteOperationsTest, testDeleteNonExistingRecordAffectsZeroRows) {
//  ITable &table = getDeleteTable();
//  std::unique_ptr<ITableRecord> nonExistingRecord = table.createRecord();
//  nonExistingRecord->getFieldValue("ID").setIntValue(-1);

//  RowsAffected nRows = table.deleteRecord(*nonExistingRecord);
//  ASSERT_EQ(nRows, 0);
//}

// TEST_F(DbDeleteOperationsTest, testDeleteMultipleRecordsByCondition) {
//  ITable &table = getDeleteTable();
//  std::unique_ptr<IFieldValue> strIndexValue = table.createFieldValue(
//      table.getField("FIELD_STR_INDEX"), std::string("STR0"));

//  std::vector<IFieldValue *> conditionValues;
//  conditionValues.push_back(&(*strIndexValue));

//  int expectedAffectedRows = (int)ceil(DELETE_TABLE_NUM_RECORDS / 9.);
//  RowsAffected nRows = table.deleteRecordsByCondition(conditionValues);
//  ASSERT_EQ(nRows, expectedAffectedRows);

//  std::unique_ptr<ITableRecordSet> recordset =
//      table.filterRecordsByFields(conditionValues);
//  ASSERT_EQ(recordset->getRecordsCount(), 0);
//}

//} // namespace unit_test
//} // namespace db

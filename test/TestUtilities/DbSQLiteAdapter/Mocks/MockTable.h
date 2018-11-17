#ifndef CPPSQLITEDBADAPTER_TEST_UTILITIES_MOCKS_MOCKTABLE_H_
#define CPPSQLITEDBADAPTER_TEST_UTILITIES_MOCKS_MOCKTABLE_H_

#include "cpp-db-adapter/ITable.h"

#include "MockTableRecordSet.h"

namespace systelab {
namespace test_utility {

class MockTable : public db::ITable {
public:
  MockTable() {}

  MockTable(const std::string &name) {
    EXPECT_CALL(*this, getName())
        .Times(AnyNumber())
        .WillRepeatedly(Return(name));
  }

  MOCK_CONST_METHOD0(getName, std::string());
  MOCK_CONST_METHOD0(getPrimaryKey, const db::IPrimaryKey &());

  MOCK_CONST_METHOD0(getFieldsCount, unsigned int());
  MOCK_CONST_METHOD1(getField, const db::IField &(unsigned int));
  MOCK_CONST_METHOD1(getField,
                     const db::IField &(const std::string &fieldName));

  MOCK_CONST_METHOD1(createFieldValueProxy,
                     db::IFieldValue *(const db::IField &));
  std::unique_ptr<db::IFieldValue>
  createFieldValue(const db::IField &field) const {
    return std::unique_ptr<db::IFieldValue>(createFieldValueProxy(field));
  }

  MOCK_CONST_METHOD2(createFieldValueProxy,
                     db::IFieldValue *(const db::IField &, bool));
  std::unique_ptr<db::IFieldValue> createFieldValue(const db::IField &field,
                                                    bool value) const {
    return std::unique_ptr<db::IFieldValue>(
        createFieldValueProxy(field, value));
  }

  MOCK_CONST_METHOD2(createFieldValueProxy,
                     db::IFieldValue *(const db::IField &, int));
  std::unique_ptr<db::IFieldValue> createFieldValue(const db::IField &field,
                                                    int value) const {
    return std::unique_ptr<db::IFieldValue>(
        createFieldValueProxy(field, value));
  }

  MOCK_CONST_METHOD2(createFieldValueProxy,
                     db::IFieldValue *(const db::IField &, double));
  std::unique_ptr<db::IFieldValue> createFieldValue(const db::IField &field,
                                                    double value) const {
    return std::unique_ptr<db::IFieldValue>(
        createFieldValueProxy(field, value));
  }

  MOCK_CONST_METHOD2(createFieldValueProxy,
                     db::IFieldValue *(const db::IField &,
                                       const std::string &));
  std::unique_ptr<db::IFieldValue>
  createFieldValue(const db::IField &field, const std::string &value) const {
    return std::unique_ptr<db::IFieldValue>(
        createFieldValueProxy(field, value));
  }

  MOCK_CONST_METHOD2(createFieldValueProxy,
                     db::IFieldValue *(const db::IField &,
                                       const boost::posix_time::ptime &));
  std::unique_ptr<db::IFieldValue>
  createFieldValue(const db::IField &field,
                   const boost::posix_time::ptime &value) const {
    return std::unique_ptr<db::IFieldValue>(
        createFieldValueProxy(field, value));
  }

  MOCK_CONST_METHOD2(createFieldValueProxy,
                     db::IFieldValue *(const db::IField &, db::IBinaryValue *));
  std::unique_ptr<db::IFieldValue>
  createFieldValue(const db::IField &field,
                   std::unique_ptr<db::IBinaryValue> value) const {
    return std::unique_ptr<db::IFieldValue>(
        createFieldValueProxy(field, value.release()));
  }

  MOCK_CONST_METHOD0(createPrimaryKeyValueProxy, db::IPrimaryKeyValue *());
  std::unique_ptr<db::IPrimaryKeyValue> createPrimaryKeyValue() const {
    return std::unique_ptr<db::IPrimaryKeyValue>(createPrimaryKeyValueProxy());
  }

  MOCK_CONST_METHOD0(getAllRecordsProxy, void());
  MOCK_CONST_METHOD0(getAllRecordsProxyNewPointer, db::ITableRecordSet *());
  std::unique_ptr<db::ITableRecordSet> getAllRecords() const {
    if (m_allMockRecordset) {
      getAllRecordsProxy();
      std::unique_ptr<MockTableRecordSet> mockRecordset =
          m_allMockRecordset->cloneMock();
      return std::unique_ptr<db::ITableRecordSet>(mockRecordset.release());
    } else {
      return std::unique_ptr<db::ITableRecordSet>(
          getAllRecordsProxyNewPointer());
    }
  }

  void
  setAllRecordsRecordSet(std::unique_ptr<MockTableRecordSet> mockRecordset) {
    m_allMockRecordset = std::move(mockRecordset);
  }

  MOCK_CONST_METHOD1(getRecordByPrimaryKeyProxy,
                     db::ITableRecord *(const db::IPrimaryKeyValue &));
  std::unique_ptr<db::ITableRecord>
  getRecordByPrimaryKey(const db::IPrimaryKeyValue &primaryKeyValue) const {
    return std::unique_ptr<db::ITableRecord>(
        getRecordByPrimaryKeyProxy(primaryKeyValue));
  }

  MOCK_CONST_METHOD2(filterRecordsByFieldProxy,
                     db::ITableRecordSet *(const db::IFieldValue &,
                                           const db::IField *));
  std::unique_ptr<db::ITableRecordSet>
  filterRecordsByField(const db::IFieldValue &conditionValue,
                       const db::IField *orderByField = NULL) const {
    return std::unique_ptr<db::ITableRecordSet>(
        filterRecordsByFieldProxy(conditionValue, orderByField));
  }

  MOCK_CONST_METHOD1(getMaxFieldValueInt, int(const db::IField &));

  MOCK_CONST_METHOD2(
      filterRecordsByFieldsProxy,
      db::ITableRecordSet *(const std::vector<db::IFieldValue *> &,
                            const db::IField *));
  std::unique_ptr<db::ITableRecordSet>
  filterRecordsByFields(const std::vector<db::IFieldValue *> &conditionValues,
                        const db::IField *orderByField = NULL) const {
    return std::unique_ptr<db::ITableRecordSet>(
        filterRecordsByFieldsProxy(conditionValues, orderByField));
  }

  MOCK_CONST_METHOD1(filterRecordsByConditionProxy,
                     db::ITableRecordSet *(const std::string &condition));
  std::unique_ptr<db::ITableRecordSet>
  filterRecordsByCondition(const std::string &condition) const {
    return std::unique_ptr<db::ITableRecordSet>(
        filterRecordsByConditionProxy(condition));
  }

  MOCK_CONST_METHOD0(createRecordProxy, db::ITableRecord *());
  std::unique_ptr<db::ITableRecord> createRecord() const {
    return std::unique_ptr<db::ITableRecord>(createRecordProxy());
  }

  MOCK_CONST_METHOD1(copyRecordProxy,
                     db::ITableRecord *(const db::ITableRecord &));
  std::unique_ptr<db::ITableRecord>
  copyRecord(const db::ITableRecord &record) const {
    return std::unique_ptr<db::ITableRecord>(copyRecordProxy(record));
  }

  MOCK_METHOD1(insertRecord, db::RowsAffected(db::ITableRecord &));
  MOCK_METHOD1(updateRecord, db::RowsAffected(const db::ITableRecord &));
  MOCK_METHOD2(updateRecord,
               db::RowsAffected(const std::vector<db::IFieldValue *> &,
                                const db::IPrimaryKeyValue &));
  MOCK_METHOD1(deleteRecord, db::RowsAffected(const db::ITableRecord &));
  MOCK_METHOD1(deleteRecord, db::RowsAffected(const db::IPrimaryKeyValue &));

  MOCK_METHOD2(updateRecordsByCondition,
               db::RowsAffected(const std::vector<db::IFieldValue *> &,
                                const std::vector<db::IFieldValue *> &));
  MOCK_METHOD1(deleteRecordsByCondition,
               db::RowsAffected(const std::vector<db::IFieldValue *> &));
  MOCK_METHOD1(deleteRecordsByCondition,
               db::RowsAffected(const std::string &condition));

  MOCK_METHOD0(deleteAllRecords, db::RowsAffected());

private:
  std::unique_ptr<MockTableRecordSet> m_allMockRecordset;
};

} // namespace test_utility
} // namespace systelab

#endif // CPP_SQLITE_DB_ADAPTER_TEST_UTILITIES_MOCKS_MOCKTABLE_H_

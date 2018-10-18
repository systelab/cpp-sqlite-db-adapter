#ifndef CPPSQLITEDBADAPTER_TEST_UTILITIES_MOCKS_MOCKTABLERECORD_H_
#define CPPSQLITEDBADAPTER_TEST_UTILITIES_MOCKS_MOCKTABLERECORD_H_

#include "cpp-db-adapter/ITable.h"
#include "cpp-db-adapter/ITableRecord.h"

#include "MockFieldValue.h"

namespace systelab {
namespace test_utility {

class MockTableRecord : public db::ITableRecord {
public:
  MockTableRecord() {}

  MockTableRecord(std::vector<std::unique_ptr<MockFieldValue>> &fieldValues) {
    unsigned int nFieldValues = fieldValues.size();
    for (unsigned int i = 0; i < nFieldValues; i++) {
      MockFieldValue *fieldValue = fieldValues[i].release();
      std::string fieldName = fieldValue->getField().getName();
      m_mockFieldValues.push_back(fieldValue);
    }
  }

  ~MockTableRecord() {
    unsigned int nFieldValues = m_mockFieldValues.size();
    for (unsigned int i = 0; i < nFieldValues; i++) {
      delete m_mockFieldValues[i];
    }
  }

  MOCK_CONST_METHOD0(getTable, db::ITable &());

  unsigned int getFieldValuesCount() const { return m_mockFieldValues.size(); }

  db::IFieldValue &getFieldValue(unsigned int index) const {
    return *m_mockFieldValues[index];
  }

  db::IFieldValue &getFieldValue(const std::string &name) const {
    unsigned int nFieldValues = m_mockFieldValues.size();
    for (unsigned int i = 0; i < nFieldValues; i++) {
      std::string fieldName = m_mockFieldValues[i]->getField().getName();
      if (fieldName == name) {
        return *m_mockFieldValues[i];
      }
    }

    throw std::runtime_error(
        std::string("Record hasn't a value for field " + name).c_str());
  }

  bool hasFieldValue(const std::string &name) const {
    unsigned int nFieldValues = m_mockFieldValues.size();
    for (unsigned int i = 0; i < nFieldValues; i++) {
      std::string fieldName = m_mockFieldValues[i]->getField().getName();
      if (fieldName == name) {
        return true;
      }
    }

    return false;
  }

  std::vector<db::IFieldValue *> getValuesList() const {
    std::vector<db::IFieldValue *> result;
    std::copy(m_mockFieldValues.begin(), m_mockFieldValues.end(),
              std::back_inserter(result));
    return result;
  }

  MOCK_METHOD1(setFieldValueProxy, void(db::IFieldValue *));
  void setFieldValue(std::unique_ptr<db::IFieldValue> field) {
    setFieldValueProxy(field.release());
  }

  std::unique_ptr<MockTableRecord> cloneMock() {
    std::vector<std::unique_ptr<MockFieldValue>> m_clonedMockFieldValues;
    unsigned int nFieldValues = m_mockFieldValues.size();
    for (unsigned int i = 0; i < nFieldValues; i++) {
      m_clonedMockFieldValues.push_back(m_mockFieldValues[i]->cloneMock());
    }

    return std::unique_ptr<MockTableRecord>(
        new MockTableRecord(m_clonedMockFieldValues));
  }

  friend bool operator==(const MockTableRecord &record1,
                         const MockTableRecord &record2) {
    if (record1.m_mockFieldValues.size() != record2.m_mockFieldValues.size())
      return false;

    unsigned int nFieldValues = record1.m_mockFieldValues.size();
    for (unsigned int i = 0; i < nFieldValues; i++) {
      MockFieldValue *fieldValue1 = record1.m_mockFieldValues.at(i);
      MockFieldValue *fieldValue2 = record2.m_mockFieldValues.at(i);
      if (*fieldValue1 != *fieldValue2)
        return false;
    }

    return true;
  }

  friend bool operator!=(const MockTableRecord &record1,
                         const MockTableRecord &record2) {
    return !(record1 == record2);
  }

private:
  std::vector<MockFieldValue *> m_mockFieldValues;
};

} // namespace test_utility
} // namespace systelab

#endif // CPP_SQLITE_DB_ADAPTER_TEST_UTILITIES_MOCKS_MOCKTABLERECORD_H_

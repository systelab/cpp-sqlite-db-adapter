#ifndef CPPSQLITEDBADAPTER_TEST_UTILITIES_MOCKS_MOCKRECORD_H_
#define CPPSQLITEDBADAPTER_TEST_UTILITIES_MOCKS_MOCKRECORD_H_

#include "cpp-db-adapter/IRecord.h"

#include "MockFieldValue.h"

namespace systelab {
namespace test_utility {

class MockRecord : public db::IRecord {
public:
  MockRecord() {}

  MockRecord(std::vector<std::unique_ptr<MockFieldValue>> &fieldValues) {
    unsigned int nFieldValues = fieldValues.size();
    for (unsigned int i = 0; i < nFieldValues; i++) {
      MockFieldValue *fieldValue = fieldValues[i].release();
      std::string fieldName = fieldValue->getField().getName();
      m_mockFieldValues.push_back(fieldValue);
    }
  }

  MockRecord::~MockRecord() {
    unsigned int nFieldValues = m_mockFieldValues.size();
    for (unsigned int i = 0; i < nFieldValues; i++) {
      delete m_mockFieldValues[i];
    }
  }

  unsigned int MockRecord::getFieldValuesCount() const {
    return m_mockFieldValues.size();
  }

  db::IFieldValue &MockRecord::getFieldValue(unsigned int index) const {
    return *m_mockFieldValues[index];
  }

  db::IFieldValue &MockRecord::getFieldValue(const std::string &name) const {
    unsigned int nFieldValues = m_mockFieldValues.size();
    for (unsigned int i = 0; i < nFieldValues; i++) {
      std::string fieldName = m_mockFieldValues[i]->getField().getName();
      if (fieldName == name) {
        return *m_mockFieldValues[i];
      }
    }

    throw std::exception(
        std::string("Record hasn't a value for field " + name).c_str());
  }

  bool MockRecord::hasFieldValue(const std::string &name) const {
    unsigned int nFieldValues = m_mockFieldValues.size();
    for (unsigned int i = 0; i < nFieldValues; i++) {
      std::string fieldName = m_mockFieldValues[i]->getField().getName();
      if (fieldName == name) {
        return true;
      }
    }

    return false;
  }

  MOCK_METHOD1(setFieldValueProxy, void(db::IFieldValue *));
  void setFieldValue(std::unique_ptr<db::IFieldValue> field) {
    setFieldValueProxy(field.release());
  }

  friend bool operator==(const MockRecord &record1, const MockRecord &record2);
  friend bool operator!=(const MockRecord &record1, const MockRecord &record2);

  std::vector<MockFieldValue *> m_mockFieldValues;
};

bool operator==(const MockRecord &record1, const MockRecord &record2) {
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

bool operator!=(const MockRecord &record1, const MockRecord &record2) {
  return !(record1 == record2);
}
} // namespace test_utility
} // namespace systelab

#endif // CPP_SQLITE_DB_ADAPTER_TEST_UTILITIES_MOCKS_MOCKRECORD_H_

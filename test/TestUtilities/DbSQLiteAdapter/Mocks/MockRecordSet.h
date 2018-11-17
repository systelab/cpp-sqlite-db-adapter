#ifndef CPPSQLITEDBADAPTER_TEST_UTILITIES_MOCKS_MOCKRECORDSET_H_
#define CPPSQLITEDBADAPTER_TEST_UTILITIES_MOCKS_MOCKRECORDSET_H_

#include "cpp-db-adapter/IRecordSet.h"

#include "MockRecord.h"

namespace systelab {
namespace test_utility {

class MockRecordSet : public db::IRecordSet {
public:
  MockRecordSet() {}

  MockRecordSet(std::vector<std::unique_ptr<MockRecord>> &records) {
    unsigned int nRecords = records.size();
    for (unsigned int i = 0; i < nRecords; i++) {
      std::unique_ptr<MockRecord> record = std::move(records[i]);
      EXPECT_CALL(*this, getCurrentRecord())
          .Times(AtMost(1))
          .WillOnce(ReturnRef(*record.get()))
          .RetiresOnSaturation();
      m_mockRecords.push_back(std::move(record));
    }
  }

  MOCK_CONST_METHOD0(getFieldsCount, unsigned int());
  MOCK_CONST_METHOD1(getField, const db::IField &(unsigned int));
  MOCK_CONST_METHOD1(getField, const db::IField &(const std::string &));

  MOCK_CONST_METHOD0(getRecordsCount, unsigned int());

  MOCK_CONST_METHOD0(getCurrentRecord, const db::IRecord &());
  MOCK_CONST_METHOD0(isCurrentRecordValid, bool());
  MOCK_METHOD0(nextRecord, void());

  std::vector<std::unique_ptr<MockRecord>> m_mockRecords;
};
} // namespace test_utility
} // namespace systelab

#endif // CPP_SQLITE_DB_ADAPTER_TEST_UTILITIES_MOCKS_MOCKRECORDSET_H_

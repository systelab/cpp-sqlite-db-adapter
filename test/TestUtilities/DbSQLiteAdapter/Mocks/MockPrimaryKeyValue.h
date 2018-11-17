#ifndef CPPSQLITEDBADAPTER_TEST_UTILITIES_MOCKS_MOCKPRIMARYKEYVALUE_H_
#define CPPSQLITEDBADAPTER_TEST_UTILITIES_MOCKS_MOCKPRIMARYKEYVALUE_H_

#include "cpp-db-adapter/IPrimaryKeyValue.h"

#include "gmock/gmock.h"

namespace systelab {
namespace test_utility {

using namespace testing;

class MockPrimaryKeyValue : public db::IPrimaryKeyValue {
public:
  MOCK_CONST_METHOD0(getTable, db::ITable &());
  MOCK_CONST_METHOD0(getPrimaryKey, db::IPrimaryKey &());

  MOCK_CONST_METHOD0(getFieldValuesCount, unsigned int());
  MOCK_CONST_METHOD1(getFieldValue, db::IFieldValue &(unsigned int));
  MOCK_CONST_METHOD1(getFieldValue, db::IFieldValue &(const std::string &));
};
} // namespace test_utility
} // namespace systelab

#endif // CPP_SQLITE_DB_ADAPTER_TEST_UTILITIES_MOCKS_MOCKPRIMARYKEYVALUE_H_

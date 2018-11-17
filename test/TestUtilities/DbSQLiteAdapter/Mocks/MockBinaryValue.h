#ifndef CPPSQLITEDBADAPTER_TEST_UTILITIES_MOCKS_MOCKBINARYVALUE_H_
#define CPPSQLITEDBADAPTER_TEST_UTILITIES_MOCKS_MOCKBINARYVALUE_H_

#include "gmock/gmock.h"

#include "cpp-db-adapter/IBinaryValue.h"

namespace systelab {
namespace test_utility {

class MockBinaryValue : public IBinaryValue {
public:
  MOCK_CONST_METHOD0(getOutputStream, std::ostream());
  MOCK_CONST_METHOD0(getInputStream, std::istream());
};
} // namespace test_utility
} // namespace systelab

#endif // CPP_SQLITE_DB_ADAPTER_TEST_UTILITIES_MOCKS_MOCKBINARYVALUE_H_

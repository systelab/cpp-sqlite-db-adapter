#ifndef CPPSQLITEDBADAPTER_TEST_UTILITIES_MOCKS_MOCKTRANSACTION_H_
#define CPPSQLITEDBADAPTER_TEST_UTILITIES_MOCKS_MOCKTRANSACTION_H_

#include "cpp-db-adapter/ITransaction.h"

#include <gmock/gmock.h>

namespace systelab {
namespace test_utility {

class MockTransaction : public db::ITransaction {
public:
  MOCK_METHOD0(commit, void());
  MOCK_METHOD0(rollback, void());
};

} // namespace test_utility
} // namespace systelab

#endif // CPP_SQLITE_DB_ADAPTER_TEST_UTILITIES_MOCKS_MOCKTRANSACTION_H_

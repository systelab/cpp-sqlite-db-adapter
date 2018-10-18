#ifndef CPPSQLITEDBADAPTER_TEST_UTILITIES_MOCKS_MOCKCONNECTION_H_
#define CPPSQLITEDBADAPTER_TEST_UTILITIES_MOCKS_MOCKCONNECTION_H_

#include "gmock/gmock.h"

#include "cpp-db-adapter/IConnection.h"

namespace systelab {
namespace test_utility {

class MockConnection : public db::IConnection {
public:
  MOCK_METHOD1(loadDatabaseProxy,
               db::IDatabase *(db::IConnectionConfiguration &));
  std::unique_ptr<db::IDatabase>
  loadDatabase(db::IConnectionConfiguration &config) {
    return std::unique_ptr<db::IDatabase>(loadDatabaseProxy(config));
  }
};

} // namespace test_utility
} // namespace systelab

#endif // CPP_SQLITE_DB_ADAPTER_TEST_UTILITIES_MOCKS_MOCKCONNECTION_H_

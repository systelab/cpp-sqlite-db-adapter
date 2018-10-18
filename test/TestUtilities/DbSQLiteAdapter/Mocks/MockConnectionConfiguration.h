#ifndef CPPSQLITEDBADAPTER_TEST_UTILITIES_MOCKS_MOCKCONNECTIONCONFIGURATION_H_
#define CPPSQLITEDBADAPTER_TEST_UTILITIES_MOCKS_MOCKCONNECTIONCONFIGURATION_H_

#include "cpp-db-adapter/IConnectionConfiguration.h"

#include "gmock/gmock.h"

namespace systelab {
namespace test_utility {

class MockConnectionConfiguration : public db::IConnectionConfiguration {
public:
  MOCK_CONST_METHOD1(getParameter,
                     std::string(const std::string &parameterName));
};

} // namespace test_utility
} // namespace systelab

#endif // CPP_SQLITE_DB_ADAPTER_TEST_UTILITIES_MOCKS_MOCKCONNECTIONCONFIGURATION_H_

#pragma once
#include "cpp-db-adapter/IConnectionConfiguration.h"

namespace systelab {
namespace db {
namespace sqlite {

class ConnectionConfiguration : public IConnectionConfiguration {
public:
  ConnectionConfiguration(const std::string &filepath);
  ~ConnectionConfiguration();

  std::string getParameter(const std::string &parameterName) const;

private:
  std::string m_filepath;
};
}
}
}

#pragma once
#include "cpp-db-adapter/IConnection.h"

namespace systelab {
namespace db {
namespace sqlite {

class Connection : public IConnection {
public:
  Connection();
  virtual ~Connection();

  std::unique_ptr<IDatabase> loadDatabase(IConnectionConfiguration &);
};
}
}
}

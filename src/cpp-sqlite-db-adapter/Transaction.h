#pragma once
#include "cpp-db-adapter/ITransaction.h"

namespace systelab {
namespace db {
class IDatabase;
}
}

namespace systelab {
namespace db {
namespace sqlite {

class Transaction : public ITransaction {
public:
  Transaction(IDatabase &database);
  virtual ~Transaction();

  void commit();
  void rollback();

private:
  IDatabase &m_database;
};
}
}
}

#ifndef CPPSQLITEDBADAPTER_CPP_SQLITE_DB_ADAPTER_BINARYVALUE_H_
#define CPPSQLITEDBADAPTER_CPP_SQLITE_DB_ADAPTER_BINARYVALUE_H_

#include "cpp-db-adapter/IBinaryValue.h"

namespace systelab {
namespace db {
namespace sqlite {

class BinaryValue : public IBinaryValue {
public:
  BinaryValue(int size);
  BinaryValue(std::istream &inputStream);
  virtual ~BinaryValue();

  std::ostream getOutputStream() const;
  std::istream getInputStream() const;

private:
  int m_size;
  char *m_buffer;
};
} // namespace sqlite
} // namespace db
} // namespace systelab

#endif // CPP_SQLITE_DB_ADAPTER_CPP_SQLITE_DB_ADAPTER_BINARYVALUE_H_

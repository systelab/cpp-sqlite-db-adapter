#ifndef CPPSQLITEDBADAPTER_TEST_UTILITIES_STUBS_STUBPRIMARYKEYVALUE_H_
#define CPPSQLITEDBADAPTER_TEST_UTILITIES_STUBS_STUBPRIMARYKEYVALUE_H_

#include "cpp-db-adapter/IPrimaryKeyValue.h"

#include <memory>
#include <vector>

using namespace systelab::db;

namespace systelab {
namespace test_utility {

class StubPrimaryKeyValue : public IPrimaryKeyValue {
public:
  StubPrimaryKeyValue(const IPrimaryKey &primaryKey);
  virtual ~StubPrimaryKeyValue();

  ITable &getTable() const;
  const IPrimaryKey &getPrimaryKey() const;

  unsigned int getFieldValuesCount() const;
  IFieldValue &getFieldValue(unsigned int index) const;
  IFieldValue &getFieldValue(const std::string &fieldName) const;

private:
  const IPrimaryKey &m_primaryKey;
  std::vector<std::unique_ptr<IFieldValue>> m_fieldValues;
};

} // namespace test_utility
} // namespace systelab

#endif // CPP_SQLITE_DB_ADAPTER_TEST_UTILITIES_STUBS_STUBPRIMARYKEYVALUE_H_

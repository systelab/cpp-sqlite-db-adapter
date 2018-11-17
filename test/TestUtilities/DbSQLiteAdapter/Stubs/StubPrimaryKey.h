#ifndef CPPSQLITEDBADAPTER_TEST_UTILITIES_STUBS_STUBPRIMARYKEY_H_
#define CPPSQLITEDBADAPTER_TEST_UTILITIES_STUBS_STUBPRIMARYKEY_H_

#include "cpp-db-adapter/IPrimaryKey.h"

#include <vector>

using namespace systelab::db;

namespace systelab {
namespace test_utility {

class StubTable;

class StubPrimaryKey : public db::IPrimaryKey {
public:
  StubPrimaryKey(StubTable &table);
  ~StubPrimaryKey() {}

  ITable &getTable() const;

  unsigned int getFieldsCount() const;
  const IField &getField(unsigned int index) const;
  const IField &getField(const std::string &fieldName) const;

  void addField(const IField &tableField);

private:
  StubTable &m_table;
  std::vector<const IField *> m_fields;
};

} // namespace test_utility
} // namespace systelab

#endif // CPP_SQLITE_DB_ADAPTER_TEST_UTILITIES_STUBS_STUBPRIMARYKEY_H_

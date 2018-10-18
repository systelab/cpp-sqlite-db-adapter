#ifndef CPP_SQLITE_DB_ADAPTER_TEST_UTILITIES_STUBS_STUBTABLERECORDSET_H_
#define CPP_SQLITE_DB_ADAPTER_TEST_UTILITIES_STUBS_STUBTABLERECORDSET_H_

#include "../Mocks/MockTableRecordSet.h"

#include "cpp-db-adapter//ITableRecord.h"
#include "cpp-db-adapter/ITableRecordSet.h"
#include "cpp-db-adapter/Types.h"

#include "StubTableRecord.h"

#include <memory>
#include <vector>

namespace systelab {
namespace db {
class IField;
class ITable;
} // namespace db
} // namespace systelab

using namespace systelab::db;

namespace systelab {
namespace test_utility {

class StubTableRecordSet : public MockTableRecordSet {
public:
  StubTableRecordSet(ITable &table, std::vector<StubTableRecord> &records);
  virtual ~StubTableRecordSet();

  ITable &getTable() const;

  unsigned int getFieldsCount() const;
  const IField &getField(unsigned int index) const;
  const IField &getField(const std::string &fieldName) const;

  unsigned int getRecordsCount() const;

  const ITableRecord &getCurrentRecord() const;
  std::unique_ptr<ITableRecord> copyCurrentRecord() const;
  bool isCurrentRecordValid() const;
  void nextRecord();

private:
  ITable &m_table;
  std::vector<std::unique_ptr<StubTableRecord>> m_records;
  std::vector<std::unique_ptr<StubTableRecord>>::iterator m_iterator;
};

} // namespace test_utility
} // namespace systelab

#endif // CPP_SQLITE_DB_ADAPTER_TEST_UTILITIES_STUBS_STUBTABLERECORDSET_H_

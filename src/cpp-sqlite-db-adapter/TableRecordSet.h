#pragma once
#include "cpp-db-adapter/ITableRecord.h"
#include "cpp-db-adapter/ITableRecordSet.h"
#include "cpp-db-adapter/Types.h"

#include <memory>
#include <vector>

struct sqlite3_stmt;

namespace systelab {
namespace db {
class IField;
class ITable;
}
}

namespace systelab {
namespace db {
namespace sqlite {

class TableRecordSet : public ITableRecordSet {
public:
  TableRecordSet(ITable &table, sqlite3_stmt *statement);
  virtual ~TableRecordSet();

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
  std::vector<std::unique_ptr<ITableRecord>> m_records;
  std::vector<std::unique_ptr<ITableRecord>>::iterator m_iterator;
};
}
}
}

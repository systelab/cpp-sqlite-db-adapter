//#include "stdafx.h"
#include "StubTableRecordSet.h"

#include "StubField.h"
#include "StubTableRecord.h"

#include "cpp-db-adapter//ITable.h"

namespace systelab {
namespace test_utility {

StubTableRecordSet::StubTableRecordSet(ITable &table,
                                       std::vector<StubTableRecord> &records)
    : m_table(table) {
  for (unsigned int i = 0; i < records.size(); i++) {
    m_records.push_back(
        std::unique_ptr<StubTableRecord>(new StubTableRecord(records[i])));
  }

  m_iterator = m_records.begin();
}

StubTableRecordSet::~StubTableRecordSet() {}

ITable &StubTableRecordSet::getTable() const { return m_table; }

unsigned int StubTableRecordSet::getFieldsCount() const {
  return m_table.getFieldsCount();
}

const IField &StubTableRecordSet::getField(unsigned int index) const {
  return m_table.getField(index);
}

const IField &StubTableRecordSet::getField(const std::string &fieldName) const {
  return m_table.getField(fieldName);
}

unsigned int StubTableRecordSet::getRecordsCount() const {
  return m_records.size();
}

const ITableRecord &StubTableRecordSet::getCurrentRecord() const {
  return *m_iterator->get();
}

std::unique_ptr<ITableRecord> StubTableRecordSet::copyCurrentRecord() const {
  const ITableRecord &currentRecord = getCurrentRecord();

  return std::unique_ptr<ITableRecord>(new StubTableRecord(currentRecord));
}

bool StubTableRecordSet::isCurrentRecordValid() const {
  return (m_iterator != m_records.end());
}

void StubTableRecordSet::nextRecord() { m_iterator++; }

} // namespace test_utility
} // namespace systelab

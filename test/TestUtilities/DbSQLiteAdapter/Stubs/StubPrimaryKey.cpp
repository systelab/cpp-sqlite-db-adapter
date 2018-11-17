//#include "stdafx.h"
#include "StubPrimaryKey.h"
#include "StubTable.h"

namespace systelab {
namespace test_utility {

StubPrimaryKey::StubPrimaryKey(StubTable &table) : m_table(table) {
  unsigned int nTableFields = m_table.getFieldsCount();
  for (unsigned int i = 0; i < nTableFields; i++) {
    const IField &tableField = m_table.getField(i);
    if (tableField.isPrimaryKey()) {
      m_fields.push_back(&tableField);
    }
  }
}

ITable &StubPrimaryKey::getTable() const { return m_table; }

unsigned int StubPrimaryKey::getFieldsCount() const { return m_fields.size(); }

const IField &StubPrimaryKey::getField(unsigned int index) const {
  if (index < m_fields.size()) {
    return *(m_fields[index]);
  } else {
    throw std::runtime_error("Invalid primary key field index");
  }
}

const IField &StubPrimaryKey::getField(const std::string &fieldName) const {
  unsigned int nFields = m_fields.size();
  for (unsigned int i = 0; i < nFields; i++) {
    if (m_fields[i]->getName() == fieldName) {
      return *(m_fields[i]);
    }
  }

  throw std::runtime_error("The requested primary key field doesn't exist");
}

void StubPrimaryKey::addField(const IField &tableField) {
  if (tableField.isPrimaryKey()) {
    m_fields.push_back(&tableField);
  }
}

} // namespace test_utility
} // namespace systelab

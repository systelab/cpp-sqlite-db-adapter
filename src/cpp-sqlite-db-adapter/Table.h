#pragma once
#include "cpp-db-adapter/ITable.h"

#include "Database.h"

namespace systelab {
namespace db {
namespace sqlite {

class Table : public ITable {
public:
  Table(Database &database, const std::string &name);
  virtual ~Table();

  std::string getName() const;
  const IPrimaryKey &getPrimaryKey() const;

  unsigned int getFieldsCount() const;
  const IField &getField(unsigned int index) const;
  const IField &getField(const std::string &fieldName) const;

  std::unique_ptr<IFieldValue> createFieldValue(const IField &) const;
  std::unique_ptr<IFieldValue> createFieldValue(const IField &, bool) const;
  std::unique_ptr<IFieldValue> createFieldValue(const IField &, int) const;
  std::unique_ptr<IFieldValue> createFieldValue(const IField &, double) const;
  std::unique_ptr<IFieldValue> createFieldValue(const IField &,
                                                const std::string &) const;
  std::unique_ptr<IFieldValue>
  createFieldValue(const IField &, const boost::posix_time::ptime &) const;
  std::unique_ptr<IFieldValue>
  createFieldValue(const IField &, std::unique_ptr<IBinaryValue>) const;

  std::unique_ptr<IPrimaryKeyValue> createPrimaryKeyValue() const;

  std::unique_ptr<ITableRecordSet> getAllRecords() const;
  std::unique_ptr<ITableRecord>
  getRecordByPrimaryKey(const IPrimaryKeyValue &) const;
  std::unique_ptr<ITableRecordSet>
  filterRecordsByField(const IFieldValue &, const IField * = NULL) const;
  std::unique_ptr<ITableRecordSet>
  filterRecordsByFields(const std::vector<IFieldValue *> &,
                        const IField * = NULL) const;
  std::unique_ptr<ITableRecordSet>
  filterRecordsByCondition(const std::string &condition) const;
  int getMaxFieldValueInt(const IField &) const;

  std::unique_ptr<ITableRecord> createRecord() const;
  std::unique_ptr<ITableRecord> copyRecord(const ITableRecord &) const;

  RowsAffected insertRecord(ITableRecord &);
  RowsAffected updateRecord(const ITableRecord &);
  RowsAffected updateRecord(const std::vector<IFieldValue *> &newValues,
                            const IPrimaryKeyValue &);
  RowsAffected deleteRecord(const ITableRecord &);
  RowsAffected deleteRecord(const IPrimaryKeyValue &);

  RowsAffected
  updateRecordsByCondition(const std::vector<IFieldValue *> &newValues,
                           const std::vector<IFieldValue *> &conditionValues);
  RowsAffected
  deleteRecordsByCondition(const std::vector<IFieldValue *> &conditionValues);
  RowsAffected deleteRecordsByCondition(const std::string &condition);

  RowsAffected deleteAllRecords();

private:
  Database &m_database;
  std::string m_name;
  std::vector<std::unique_ptr<IField>> m_fields;
  std::unique_ptr<IPrimaryKey> m_primaryKey;

  void loadFields();
  FieldTypes getTypeFromSQLiteTypeName(std::string typeName);

  bool isOwned(const IField &field) const;
  std::string getSQLValue(const IFieldValue &fieldValue, bool forComparison,
                          bool forAssignment) const;
  std::string getStringList(const std::vector<std::string> &items,
                            const std::string &separator) const;
};
}
}
}

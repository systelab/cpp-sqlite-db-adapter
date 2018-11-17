#ifndef CPP_SQLITE_DB_ADAPTER_TEST_UTILITIES_STUBS_STUBTABLERECORD_H_
#define CPP_SQLITE_DB_ADAPTER_TEST_UTILITIES_STUBS_STUBTABLERECORD_H_

#include "../Mocks/MockTableRecord.h"
#include "../Stubs/StubFieldValue.h"

#include <memory>
#include <vector>

namespace systelab {
namespace test_utility {

class StubField;

class StubTableRecord : public MockTableRecord {
public:
  StubTableRecord(const ITableRecord &);
  StubTableRecord(const StubTableRecord &);
  StubTableRecord(std::vector<std::unique_ptr<StubFieldValue>> &);
  virtual ~StubTableRecord();

  db::ITable &getTable() const;
  unsigned int getFieldValuesCount() const;
  StubFieldValue &getFieldValue(unsigned int index) const;
  db::IFieldValue &getFieldValue(const std::string &fieldName) const;

  bool hasFieldValue(const std::string &fieldName) const;

  std::vector<db::IFieldValue *> getValuesList() const;

  StubTableRecord &operator=(const StubTableRecord &other);

private:
  std::vector<std::unique_ptr<StubFieldValue>> m_fieldValues;
};

} // namespace test_utility
} // namespace systelab

#endif // CPP_SQLITE_DB_ADAPTER_TEST_UTILITIES_STUBS_STUBTABLERECORD_H_

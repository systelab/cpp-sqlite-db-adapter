#ifndef CPPSQLITEDBADAPTER_TEST_UTILITIES_STUBS_STUBFIELDVALUE_H_
#define CPPSQLITEDBADAPTER_TEST_UTILITIES_STUBS_STUBFIELDVALUE_H_

#include "cpp-db-adapter//IFieldValue.h"

#include <boost/optional.hpp>
#include <memory>
#include <string>

namespace systelab {
namespace db {
class IBinaryValue;
class IField;
} // namespace db
} // namespace systelab

namespace systelab {
namespace test_utility {

class StubField;

class StubFieldValue : public db::IFieldValue {
public:
  StubFieldValue(const db::IFieldValue &);
  StubFieldValue(const StubFieldValue &);
  StubFieldValue(const std::string &name);
  StubFieldValue(const std::string &name, int value);
  StubFieldValue(const std::string &name, bool value);
  StubFieldValue(const std::string &name, double value);
  StubFieldValue(const std::string &name, const std::string &value);
  StubFieldValue(const std::string &name,
                 const boost::posix_time::ptime &value);

  StubFieldValue(const std::string &name, const boost::optional<int> &value);
  StubFieldValue(const std::string &name, const boost::optional<bool> &value);
  StubFieldValue(const std::string &name, const boost::optional<double> &value);
  StubFieldValue(const std::string &name,
                 const boost::optional<std::string> &value);
  StubFieldValue(const std::string &name,
                 const boost::optional<boost::posix_time::ptime> &value);

  virtual ~StubFieldValue();

  const db::IField &getField() const;
  bool isNull() const;
  bool isDefault() const;

  bool getBooleanValue() const;
  int getIntValue() const;
  double getDoubleValue() const;
  std::string getStringValue() const;
  boost::posix_time::ptime getDateTimeValue() const;
  db::IBinaryValue &getBinaryValue() const;

  void setValue(const db::IFieldValue &);
  void setNull();
  void setDefault();
  void setBooleanValue(bool);
  void setIntValue(int);
  void setDoubleValue(double);
  void setStringValue(const std::string &);
  void setDateTimeValue(const boost::posix_time::ptime &);
  void setBinaryValue(std::unique_ptr<db::IBinaryValue>);

  void useDefaultValue();
  std::unique_ptr<db::IFieldValue> clone() const;

  StubFieldValue &operator=(const StubFieldValue &other);

private:
  std::unique_ptr<StubField> m_field;
  bool m_nullValue;
  bool m_default;
  bool m_boolValue;
  int m_intValue;
  double m_doubleValue;
  std::string m_stringValue;
  boost::posix_time::ptime m_dateTimeValue;
};

} // namespace test_utility
} // namespace systelab

#endif // CPP_SQLITE_DB_ADAPTER_TEST_UTILITIES_STUBS_STUBFIELDVALUE_H_

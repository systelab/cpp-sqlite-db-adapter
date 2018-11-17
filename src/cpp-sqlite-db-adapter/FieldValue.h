#pragma once
#include "cpp-db-adapter/IFieldValue.h"

#include <memory>

namespace systelab {
namespace db {
namespace sqlite {

class FieldValue : public IFieldValue {
public:
  FieldValue(const IField &);
  FieldValue(const IField &, bool);
  FieldValue(const IField &, int);
  FieldValue(const IField &, double);
  FieldValue(const IField &, const std::string &);
  FieldValue(const IField &, const boost::posix_time::ptime &);
  FieldValue(const IField &, std::unique_ptr<IBinaryValue>);
  virtual ~FieldValue(void);

  const IField &getField() const;

  bool isNull() const;
  bool isDefault() const;
  bool getBooleanValue() const;
  int getIntValue() const;
  double getDoubleValue() const;
  std::string getStringValue() const;
  boost::posix_time::ptime getDateTimeValue() const;
  IBinaryValue &getBinaryValue() const;

  void setValue(const IFieldValue &);
  void setNull();
  void setDefault();
  void setBooleanValue(bool value);
  void setIntValue(int value);
  void setDoubleValue(double value);
  void setStringValue(const std::string &value);
  void setDateTimeValue(const boost::posix_time::ptime &value);
  void setBinaryValue(std::unique_ptr<IBinaryValue> value);

  void useDefaultValue();

  std::unique_ptr<IFieldValue> clone() const;

private:
  const IField &m_field;

  bool m_nullValue;
  bool m_default;
  bool m_boolValue;
  int m_intValue;
  double m_doubleValue;
  std::string m_stringValue;
  boost::posix_time::ptime m_dateTimeValue;
  std::unique_ptr<IBinaryValue> m_binaryValue;
};
}
}
}

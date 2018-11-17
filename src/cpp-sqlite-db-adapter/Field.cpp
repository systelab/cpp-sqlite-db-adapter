//#include "stdafx.h"
#include "Field.h"
#include <algorithm>
//#include <exception>
//#include <iostream>

namespace systelab {
namespace db {
namespace sqlite {

Field::Field(unsigned int index, const std::string &name, FieldTypes type,
             const std::string &defaultValue, bool primaryKey)
    : m_index(index), m_name(name), m_type(type), m_primaryKey(primaryKey) {
  setDefaultValue(type, defaultValue);
}

Field::~Field() {}

unsigned int Field::getIndex() const { return m_index; }

std::string Field::getName() const { return m_name; }

FieldTypes Field::getType() const { return m_type; }

bool Field::hasNullDefaultValue() const { return m_nullDefaultValue; }

bool Field::getBooleanDefaultValue() const {
  if (!hasNullDefaultValue()) {
    if (m_type == BOOLEAN) {
      return m_defaultBoolValue;
    } else {
      throw std::string("Field type isn't boolean");
    }
  } else {
    throw std::string("Default value is null");
  }
}

int Field::getIntDefaultValue() const {
  if (!hasNullDefaultValue()) {
    if (m_type == INT) {
      return m_defaultIntValue;
    } else {
      throw std::string("Field type isn't integer");
    }
  } else {
    throw std::string("Default value is null");
  }
}

double Field::getDoubleDefaultValue() const {
  if (!hasNullDefaultValue()) {
    if (m_type == DOUBLE) {
      return m_defaultDoubleValue;
    } else {
      throw std::string("Field type isn't double");
    }
  } else {
    throw std::string("Default value is null");
  }
}

std::string Field::getStringDefaultValue() const {
  if (!hasNullDefaultValue()) {
    if (m_type == STRING) {
      return m_defaultStringValue;
    } else {
      throw std::string("Field type isn't string");
    }
  } else {
    throw std::string("Default value is null");
  }
}

boost::posix_time::ptime Field::getDateTimeDefaultValue() const {
  if (!hasNullDefaultValue()) {
    if (m_type == DATETIME) {
      return m_defaultDateTimeValue;
    } else {
      throw std::string("Field type isn't datetime");
    }
  } else {
    throw std::string("Default value is null");
  }
}

IBinaryValue &Field::getBinaryDefaultValue() const {
  if (!hasNullDefaultValue()) {
    if (m_type == BINARY) {
      return *m_defaultBinaryValue.get();
    } else {
      throw std::string("Field type isn't binary");
    }
  } else {
    throw std::string("Default value is null");
  }
}

bool Field::isPrimaryKey() const { return m_primaryKey; }

void Field::setDefaultValue(FieldTypes type, const std::string &defaultValue) {
  m_defaultBoolValue = false;
  m_defaultIntValue = 0;
  m_defaultDoubleValue = 0.;
  m_defaultStringValue = "";
  m_defaultDateTimeValue = boost::posix_time::ptime();
  m_defaultBinaryValue.reset();

  std::string defaultValueUpper = defaultValue;
  std::transform(defaultValueUpper.begin(), defaultValueUpper.end(),
                 defaultValueUpper.begin(), ::toupper);
  if (defaultValue.empty() || defaultValueUpper == "NULL") {
    m_nullDefaultValue = true;
  } else {
    m_nullDefaultValue = false;
    switch (type) {
    case BOOLEAN:
      m_defaultBoolValue = (std::stoi(defaultValue) == 1);
      break;
    case INT:
      m_defaultIntValue = std::stoi(defaultValue);
      break;
    case DOUBLE:
      m_defaultDoubleValue = std::stod(defaultValue);
      break;
    case STRING:
      m_defaultStringValue = defaultValue;
      break;
    case DATETIME:
      m_defaultDateTimeValue = getDateTimeFromSQLiteString(defaultValue);
      break;
    case BINARY:
      // m_defaultBinaryValue = ; // Not implemented
      break;
    default:
      throw std::string("Invalid record field type.");
      break;
    }
  }
}

boost::posix_time::ptime
Field::getDateTimeFromSQLiteString(const std::string &sqliteDateTime) const {
  if (!sqliteDateTime.empty()) {
    std::string sqliteDateTimeNoQuotes = sqliteDateTime.substr(1);
    sqliteDateTimeNoQuotes =
        sqliteDateTimeNoQuotes.substr(0, sqliteDateTimeNoQuotes.length() - 1);
    return boost::posix_time::from_iso_string(sqliteDateTimeNoQuotes);
  } else {
    return boost::posix_time::ptime();
  }
}
}
}
}

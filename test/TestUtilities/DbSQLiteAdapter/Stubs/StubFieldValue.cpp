#include "stdafx.h"
#include "StubFieldValue.h"

#include "StubField.h"


namespace systelab { namespace test_utility {

	StubFieldValue::StubFieldValue(const db::IFieldValue& other)
		:m_nullValue(other.isNull())
		,m_default(other.isDefault())
		,m_boolValue(false)
		,m_intValue(0)
		,m_doubleValue(0.)
		,m_stringValue("")
		,m_dateTimeValue()
	{
		m_field.reset(new StubField(other.getField()));
		
		if (!other.isNull())
		{
			switch (m_field->getType())
			{
				case db::BOOLEAN:
					setBooleanValue(other.getBooleanValue());
					break;
				case db::INT:
					setIntValue(other.getIntValue());
					break;
				case db::DOUBLE:
					setDoubleValue(other.getDoubleValue());
					break;
				case db::STRING:
					setStringValue(other.getStringValue());
					break;
				case db::DATETIME:
					setDateTimeValue(other.getDateTimeValue());
					break;
				case db::BINARY:
					//setBinaryValue(other.getBinaryValue()); // Not implemented
					break;
			}
		}
	}

	StubFieldValue::StubFieldValue(const StubFieldValue& other)
		:m_nullValue(other.m_nullValue)
		,m_default(other.m_default)
		,m_boolValue(other.m_boolValue)
		,m_intValue(other.m_intValue)
		,m_doubleValue(other.m_doubleValue)
		,m_stringValue(other.m_stringValue)
		,m_dateTimeValue(other.m_dateTimeValue)
	{
		m_field.reset(new StubField(*other.m_field));
	}

	StubFieldValue::StubFieldValue(const std::string& name)
		:m_nullValue(true)
		,m_default(false)
		,m_boolValue(false)
		,m_intValue(0)
		,m_doubleValue(0.)
		,m_stringValue("")
		,m_dateTimeValue()
	{
		m_field.reset(new StubField(name, db::INT));
	}

	StubFieldValue::StubFieldValue(const std::string& name, int value)
		:m_nullValue(false)
		,m_default(false)
		,m_boolValue(false)
		,m_intValue(value)
		,m_doubleValue(0.)
		,m_stringValue("")
		,m_dateTimeValue()
	{
		m_field.reset(new StubField(name, db::INT));
	}

	StubFieldValue::StubFieldValue(const std::string& name, bool value)
		:m_nullValue(false)
		,m_default(false)
		,m_boolValue(value)
		,m_intValue(0)
		,m_doubleValue(0.)
		,m_stringValue("")
		,m_dateTimeValue()
	{
		m_field.reset(new StubField(name, db::BOOLEAN));
	}

	StubFieldValue::StubFieldValue(const std::string& name, double value)
		:m_nullValue(false)
		,m_default(false)
		,m_boolValue(false)
		,m_intValue(0)
		,m_doubleValue(value)
		,m_stringValue("")
		,m_dateTimeValue()
	{
		m_field.reset(new StubField(name, db::DOUBLE));
	}

	StubFieldValue::StubFieldValue(const std::string& name, const std::string& value)
		:m_nullValue(false)
		,m_default(false)
		,m_boolValue(false)
		,m_intValue(0)
		,m_doubleValue(0.)
		,m_stringValue(value)
		,m_dateTimeValue()
	{
		m_field.reset(new StubField(name, db::STRING));
	}

	StubFieldValue::StubFieldValue(const std::string& name, const boost::posix_time::ptime& value)
		:m_nullValue(false)
		,m_default(false)
		,m_boolValue(false)
		,m_intValue(0)
		,m_doubleValue(0.)
		,m_stringValue("")
		,m_dateTimeValue(value)
	{
		m_field.reset(new StubField(name, db::DATETIME));
	}

	StubFieldValue::StubFieldValue(const std::string& name, const boost::optional<int>& value)
		:m_default(false)
		,m_boolValue(false)
		,m_doubleValue(0.)
		,m_stringValue("")
		,m_dateTimeValue()
	{
		m_field.reset(new StubField(name, db::INT));
			
		if (value)
		{
			m_intValue = value.get();
			m_nullValue = false;
		}
		else
		{
			m_intValue = 0;
			m_nullValue = true;
		}
	}

	StubFieldValue::StubFieldValue(const std::string& name, const boost::optional<bool>& value)
		:m_default(false)
		,m_intValue(0)
		,m_doubleValue(0.)
		,m_stringValue("")
		,m_dateTimeValue()
	{
		m_field.reset(new StubField(name, db::BOOLEAN));

		if (value)
		{
			m_boolValue = value.get();
			m_nullValue = false;
		}
		else
		{
			m_boolValue = false;
			m_nullValue = true;
		}
	}
		
	StubFieldValue::StubFieldValue(const std::string& name, const boost::optional<double>& value)
		:m_default(false)
		,m_intValue(0)
		,m_boolValue(false)
		,m_stringValue("")
		,m_dateTimeValue()
	{
		m_field.reset(new StubField(name, db::DOUBLE));

		if (value)
		{
			m_doubleValue = value.get();
			m_nullValue = false;
		}
		else
		{
			m_doubleValue = 0.;
			m_nullValue = true;
		}
	}

	StubFieldValue::StubFieldValue(const std::string& name, const boost::optional<std::string>& value)
		:m_default(false)
		,m_intValue(0)
		,m_boolValue(false)
		,m_doubleValue(0.)
		,m_dateTimeValue()
	{
		m_field.reset(new StubField(name, db::DOUBLE));
			
		if (value)
		{
			m_stringValue = value.get();
			m_nullValue = false;
		}
		else
		{
			m_stringValue = "";
			m_nullValue = true;
		}
	}


	StubFieldValue::StubFieldValue(const std::string& name, const boost::optional<boost::posix_time::ptime>& value)
		:m_default(false)
		,m_intValue(0)
		,m_boolValue(false)
		,m_doubleValue(0.)
		,m_dateTimeValue()
	{
		m_field.reset(new StubField(name, db::DATETIME));
		if (value)
		{
			m_dateTimeValue = value.get();
			m_nullValue = false;
		}
		else
		{
			m_dateTimeValue = boost::posix_time::ptime();
			m_nullValue = true;
		}

	}

	StubFieldValue::~StubFieldValue()
	{
	}

	const db::IField& StubFieldValue::getField() const
	{
		return *m_field;
	}

	bool StubFieldValue::isNull() const
	{
		return m_nullValue;
	}

	bool StubFieldValue::isDefault() const
	{
		return m_default;
	}

	bool StubFieldValue::getBooleanValue() const
	{
		if (!isNull() && !isDefault())
		{
			if( m_field->getType() == db::BOOLEAN )
			{
				return m_boolValue;
			}
			else
			{
				throw std::exception( "Field type isn't boolean" );
			}
		}
		else
		{
			if (isNull())
			{
				throw std::exception( "Field value is null" );
			}
			else
			{
				throw std::exception( "Field value is default" );
			}
		}
	}

	int StubFieldValue::getIntValue() const
	{
		if (!isNull() && !isDefault())
		{
			if( m_field->getType() == db::INT )
			{
				return m_intValue;
			}
			else
			{
				throw std::exception( "Field type isn't integer" );
			}
		}
		else
		{
			if (isNull())
			{
				throw std::exception( "Field value is null" );
			}
			else
			{
				throw std::exception( "Field value is default" );
			}
		}
	}

	double StubFieldValue::getDoubleValue() const
	{
		if (!isNull() && !isDefault())
		{
			if( m_field->getType() == db::DOUBLE )
			{
				return m_doubleValue;
			}
			else
			{
				throw std::exception( "Field type isn't double" );
			}
		}
		else
		{
			if (isNull())
			{
				throw std::exception( "Field value is null" );
			}
			else
			{
				throw std::exception( "Field value is default" );
			}
		}
	}

	std::string StubFieldValue::getStringValue() const
	{
		if (!isNull() && !isDefault())
		{
			if( m_field->getType() == db::STRING )
			{
				return m_stringValue;
			}
			else
			{
				throw std::exception( "Field type isn't string" );
			}
		}
		else
		{
			if (isNull())
			{
				throw std::exception( "Field value is null" );
			}
			else
			{
				throw std::exception( "Field value is default" );
			}
		}
	}
		

	boost::posix_time::ptime StubFieldValue::getDateTimeValue() const
	{
		if (!isDefault())
		{
			if( m_field->getType() == db::DATETIME )
			{
				return m_dateTimeValue;
			}
			else if ( m_field->getType() == db::STRING )
			{
				if (!isNull())
				{
					return boost::posix_time::from_iso_string(m_stringValue);
				}
				else
				{
					return boost::posix_time::ptime();
				}
			}
			else
			{
				throw std::exception( "Field type isn't datetime" );
			}
		}
		else
		{
			throw std::exception( "Field value is default" );
		}
	}

	db::IBinaryValue& StubFieldValue::getBinaryValue() const
	{
		throw std::exception( "Not implemented" );
	}

	void StubFieldValue::setValue(const db::IFieldValue& srcFieldValue)
	{
		db::FieldTypes srcFieldType = srcFieldValue.getField().getType();
		if (srcFieldType != getField().getType())
		{
			throw std::exception( "Can't set the value of a field of another type" );
		}

		if (srcFieldValue.isNull())
		{
			setNull();
		}
		else if (srcFieldValue.isDefault())
		{
			setDefault();
		}
		else
		{
			switch (srcFieldType)
			{
				case db::BOOLEAN:
					setBooleanValue(srcFieldValue.getBooleanValue());
					break;
				case db::INT:
					setIntValue(srcFieldValue.getIntValue());
					break;
				case db::DOUBLE:
					setDoubleValue(srcFieldValue.getDoubleValue());
					break;
				case db::STRING:
					setStringValue(srcFieldValue.getStringValue());
					break;
				case db::DATETIME:
					setDateTimeValue(srcFieldValue.getDateTimeValue());
					break;
				case db::BINARY:
					//setBinaryValue(srcFieldValue.getBinaryValue()); // Not implemented
					break;
			}
		}
	}

	void StubFieldValue::setNull()
	{
		m_nullValue = true;
		m_default = false;
		m_boolValue = false;
		m_intValue = 0;
		m_doubleValue = 0.;
		m_stringValue = "";
		m_dateTimeValue = boost::posix_time::ptime();
	}

	void StubFieldValue::setDefault()
	{
		m_nullValue = false;
		m_default = true;
		m_boolValue = false;
		m_intValue = 0;
		m_doubleValue = 0.;
		m_stringValue = "";
		m_dateTimeValue = boost::posix_time::ptime();
	}

	void StubFieldValue::setBooleanValue(bool value)
	{
		if( m_field->getType() == db::BOOLEAN )
		{
			m_boolValue = value;
			m_nullValue = false;
			m_default = false;
		}
		else
		{
			throw std::exception( "Field type isn't boolean" );
		}
	}

	void StubFieldValue::setIntValue(int value)
	{
		if( m_field->getType() == db::INT )
		{
			m_intValue = value;
			m_nullValue = false;
			m_default = false;
		}
		else
		{
			throw std::exception( "Field type isn't integer" );
		}
	}

	void StubFieldValue::setDoubleValue(double value)
	{
		if( m_field->getType() == db::DOUBLE )
		{
			m_doubleValue = value;
			m_nullValue = false;
			m_default = false;
		}
		else
		{
			throw std::exception( "Field type isn't double" );
		}
	}

	void StubFieldValue::setStringValue(const std::string& value)
	{
		if( m_field->getType() == db::STRING )
		{
			m_stringValue = value;
			m_nullValue = false;
			m_default = false;
		}
		else
		{
			throw std::exception( "Field type isn't string" );
		}
	}

	void StubFieldValue::setDateTimeValue(const boost::posix_time::ptime& value)
	{
		if( m_field->getType() == db::DATETIME )
		{
			m_dateTimeValue = value;
			m_nullValue = value.is_not_a_date_time();
			m_default = false;
		}
		else
		{
			throw std::exception( "Field type isn't datetime" );
		}
	}

	void StubFieldValue::setBinaryValue(std::unique_ptr<db::IBinaryValue> value)
	{
		throw std::exception( "Not implemented" );
	}

	void StubFieldValue::useDefaultValue()
	{
		db::FieldTypes fieldType = m_field->getType();
		switch (fieldType)
		{
			case db::BOOLEAN:
				setBooleanValue(m_field->getBooleanDefaultValue());
				break;

			case db::INT:
				setIntValue(m_field->getIntDefaultValue());
				break;

			case db::DOUBLE:
				setDoubleValue(m_field->getDoubleDefaultValue());
				break;

			case db::STRING:
				setStringValue(m_field->getStringDefaultValue());
				break;

			case db::DATETIME:
				setDateTimeValue(m_field->getDateTimeDefaultValue());
				break;

			case db::BINARY:
				throw std::exception( "Binary field type not implemented." );

			default:
				throw std::exception( "Invalid field type." );
				break;
		}
	}

	std::unique_ptr<db::IFieldValue> StubFieldValue::clone() const
	{
		return std::unique_ptr<db::IFieldValue>(new StubFieldValue(*this));
	}

	StubFieldValue& StubFieldValue::operator= (const StubFieldValue& other)
	{
		m_field.reset(new StubField(*other.m_field));
		m_nullValue = other.m_nullValue;
		m_default = other.m_default;
		m_boolValue = other.m_boolValue;
		m_intValue = other.m_intValue;
		m_doubleValue = other.m_doubleValue;
		m_stringValue = other.m_stringValue;
		m_dateTimeValue = other.m_dateTimeValue;

		return *this;
	}

}}


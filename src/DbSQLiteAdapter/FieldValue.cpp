#include "FieldValue.h"

#include "BinaryValue.h"


namespace systelab { namespace db { namespace sqlite {

	FieldValue::FieldValue(const IField& field)
		:m_field(field)
		,m_nullValue(true)
		,m_default(false)
		,m_boolValue(false)
		,m_intValue(0)
		,m_doubleValue(0.)
		,m_stringValue("")
		,m_dateTimeValue()
		,m_binaryValue()
	{
	}

	FieldValue::FieldValue(const IField& field, bool value)
		:m_field(field)
		,m_nullValue(false)
		,m_default(false)
		,m_boolValue(value)
		,m_intValue(0)
		,m_doubleValue(0.)
		,m_stringValue("")
		,m_dateTimeValue()
		,m_binaryValue()
	{
		if (m_field.getType() != BOOLEAN)
		{
			throw std::runtime_error( "Field doesn't accept a boolean value" );
		}
	}

	FieldValue::FieldValue(const IField& field, int value)
		:m_field(field)
		,m_nullValue(false)
		,m_default(false)
		,m_boolValue(false)
		,m_intValue(value)
		,m_doubleValue(0.)
		,m_stringValue("")
		,m_dateTimeValue()
		,m_binaryValue()
	{
		if (m_field.getType() != INT)
		{
			throw std::runtime_error( "Field doesn't accept an integer value" );
		}
	}

	FieldValue::FieldValue(const IField& field, double value)
		:m_field(field)
		,m_nullValue(false)
		,m_default(false)
		,m_boolValue(false)
		,m_intValue(0)
		,m_doubleValue(value)
		,m_stringValue("")
		,m_dateTimeValue()
		,m_binaryValue()
	{
		if (m_field.getType() != DOUBLE)
		{
			throw std::runtime_error( "Field doesn't accept a double value" );
		}
	}

	FieldValue::FieldValue(const IField& field, const std::string& value)
		:m_field(field)
		,m_nullValue(false)
		,m_default(false)
		,m_boolValue(false)
		,m_intValue(0)
		,m_doubleValue(0.)
		,m_stringValue(value)
		,m_dateTimeValue()
		,m_binaryValue()
	{
		if (m_field.getType() != STRING)
		{
			throw std::runtime_error( "Field doesn't accept a string value" );
		}
	}

	FieldValue::FieldValue(const IField& field, const boost::posix_time::ptime& value)
		:m_field(field)
		,m_nullValue(true)
		,m_default(false)
		,m_boolValue(false)
		,m_intValue(0)
		,m_doubleValue(0.)
		,m_stringValue("")
		,m_dateTimeValue()
		,m_binaryValue()
	{
		if (m_field.getType() != DATETIME)
		{
			throw std::runtime_error( "Field doesn't accept a datetime value" );
		}

		if (!value.is_not_a_date_time())
		{
			m_dateTimeValue = value;
			m_nullValue = false;
		}
	}

	FieldValue::FieldValue(const IField& field, std::unique_ptr<IBinaryValue> value)
		:m_field(field)
		,m_nullValue(false)
		,m_default(false)
		,m_boolValue(false)
		,m_intValue(0)
		,m_doubleValue(0.)
		,m_stringValue("")
		,m_dateTimeValue()
		,m_binaryValue(std::move(value))
	{
		if (m_field.getType() != BINARY)
		{
			throw std::runtime_error( "Field doesn't accept a binary value" );
		}
	}

	FieldValue::~FieldValue()
	{
	}

	const IField& FieldValue::getField() const
	{
		return m_field;
	}

	bool FieldValue::isNull() const
	{
		return m_nullValue;
	}

	bool FieldValue::isDefault() const
	{
		return m_default;
	}

	bool FieldValue::getBooleanValue() const
	{
		if (!isNull() && !isDefault())
		{
			if( m_field.getType() == BOOLEAN )
			{
				return m_boolValue;
			}
			else
			{
				throw std::runtime_error( "Field type isn't boolean" );
			}
		}
		else
		{
			if (isNull())
			{
				throw std::runtime_error( "Field value is null" );
			}
			else
			{
				throw std::runtime_error( "Field value is default" );
			}
		}
	}

	int FieldValue::getIntValue() const
	{
		if (!isNull() && !isDefault())
		{
			if( m_field.getType() == INT )
			{
				return m_intValue;
			}
			else
			{
				throw std::runtime_error( "Field type isn't integer" );
			}
		}
		else
		{
			if (isNull())
			{
				throw std::runtime_error( "Field value is null" );
			}
			else
			{
				throw std::runtime_error( "Field value is default" );
			}
		}
	}

	double FieldValue::getDoubleValue() const
	{
		if (!isNull() && !isDefault())
		{
			if( m_field.getType() == DOUBLE )
			{
				return m_doubleValue;
			}
			else
			{
				throw std::runtime_error( "Field type isn't double" );
			}
		}
		else
		{
			if (isNull())
			{
				throw std::runtime_error( "Field value is null" );
			}
			else
			{
				throw std::runtime_error( "Field value is default" );
			}
		}
	}

	std::string FieldValue::getStringValue() const
	{
		if (!isNull() && !isDefault())
		{
			if( m_field.getType() == STRING )
			{
				return m_stringValue;
			}
			else
			{
				throw std::runtime_error( "Field type isn't string" );
			}
		}
		else
		{
			if (isNull())
			{
				throw std::runtime_error( "Field value is null" );
			}
			else
			{
				throw std::runtime_error( "Field value is default" );
			}
		}
	}

	boost::posix_time::ptime FieldValue::getDateTimeValue() const
	{
		if (!isDefault())
		{
			if( m_field.getType() == DATETIME )
			{
				return m_dateTimeValue;
			}
			else if ( m_field.getType() == STRING )
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
				throw std::runtime_error( "Field type isn't datetime" );
			}
		}
		else
		{
			throw std::runtime_error( "Field value is default" );
		}
	}

	IBinaryValue& FieldValue::getBinaryValue() const
	{
		if (!isNull() && !isDefault())
		{
			if( m_field.getType() == BINARY )
			{
				return *m_binaryValue.get();
			}
			else
			{
				throw std::runtime_error( "Field type isn't binary" );
			}
		}
		else
		{
			if (isNull())
			{
				throw std::runtime_error( "Field value is null" );
			}
			else
			{
				throw std::runtime_error( "Field value is default" );
			}
		}
	}

	void FieldValue::setValue(const IFieldValue& srcFieldValue)
	{
		FieldTypes srcFieldType = srcFieldValue.getField().getType();
		if (srcFieldType != getField().getType())
		{
			throw std::runtime_error( "Can't set the value of a field of another type" );
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
				case BOOLEAN:
					setBooleanValue(srcFieldValue.getBooleanValue());
					break;
				case INT:
					setIntValue(srcFieldValue.getIntValue());
					break;
				case DOUBLE:
					setDoubleValue(srcFieldValue.getDoubleValue());
					break;
				case STRING:
					setStringValue(srcFieldValue.getStringValue());
					break;
				case DATETIME:
					setDateTimeValue(srcFieldValue.getDateTimeValue());
					break;
				case BINARY:
					//setBinaryValue(srcFieldValue.getBinaryValue()); // Not implemented
					break;
			}
		}
	}

	void FieldValue::setNull()
	{
		m_nullValue = true;
		m_default = false;
		m_boolValue = false;
		m_intValue = 0;
		m_doubleValue = 0.;
		m_stringValue = "";
		m_dateTimeValue = boost::posix_time::ptime();
		m_binaryValue.reset();
	}

	void FieldValue::setDefault()
	{
		m_nullValue = false;
		m_default = true;
		m_boolValue = false;
		m_intValue = 0;
		m_doubleValue = 0.;
		m_stringValue = "";
		m_dateTimeValue = boost::posix_time::ptime();
		m_binaryValue.reset();
	}

	void FieldValue::setBooleanValue(bool value)
	{
		if( m_field.getType() == BOOLEAN )
		{
			m_boolValue = value;
			m_nullValue = false;
			m_default = false;
		}
		else
		{
			throw std::runtime_error( "Field type isn't boolean" );
		}
	}

	void FieldValue::setIntValue(int value)
	{
		if( m_field.getType() == INT )
		{
			m_intValue = value;
			m_nullValue = false;
			m_default = false;
		}
		else
		{
			throw std::runtime_error( "Field type isn't integer" );
		}
	}

	void FieldValue::setDoubleValue(double value)
	{
		if( m_field.getType() == DOUBLE )
		{
			m_doubleValue = value;
			m_nullValue = false;
			m_default = false;
		}
		else
		{
			throw std::runtime_error( "Field type isn't double" );
		}
	}

	void FieldValue::setStringValue(const std::string& value)
	{
		if( m_field.getType() == STRING )
		{
			m_stringValue = value;
			m_nullValue = false;
			m_default = false;
		}
		else
		{
			throw std::runtime_error( "Field type isn't string" );
		}
	}

	void FieldValue::setDateTimeValue(const boost::posix_time::ptime& value)
	{
		if( m_field.getType() == DATETIME )
		{
			m_dateTimeValue = value;
			m_nullValue = value.is_not_a_date_time();
			m_default = false;
		}
		else
		{
			throw std::runtime_error( "Field type isn't datetime" );
		}
	}

	void FieldValue::setBinaryValue(std::unique_ptr<IBinaryValue> value)
	{
		if( m_field.getType() == DATETIME )
		{
			m_binaryValue = std::move(value);
			m_nullValue = false;
			m_default = false;
		}
		else
		{
			throw std::runtime_error( "Field type isn't binary" );
		}
	}

	void FieldValue::useDefaultValue()
	{
		FieldTypes fieldType = m_field.getType();
		switch (fieldType)
		{
			case BOOLEAN:
				setBooleanValue(m_field.getBooleanDefaultValue());
				break;

			case INT:
				setIntValue(m_field.getIntDefaultValue());
				break;

			case DOUBLE:
				setDoubleValue(m_field.getDoubleDefaultValue());
				break;

			case STRING:
				setStringValue(m_field.getStringDefaultValue());
				break;

			case DATETIME:
				setDateTimeValue(m_field.getDateTimeDefaultValue());
				break;

			case BINARY:
				throw std::runtime_error( "Binary field type not implemented." );

			default:
				throw std::runtime_error( "Invalid field type." );
				break;
		}
	}

	std::unique_ptr<IFieldValue> FieldValue::clone() const
	{
		if (isNull())
		{
			return std::unique_ptr<IFieldValue>( new FieldValue(m_field) );
		}
		else if (isDefault())
		{
			std::unique_ptr<IFieldValue> fieldValue( new FieldValue(m_field) );
			fieldValue->setDefault();
			return fieldValue;
		}
		else
		{
			FieldTypes fieldType = m_field.getType();
			switch (fieldType)
			{
				case BOOLEAN:
					return std::unique_ptr<IFieldValue>( new FieldValue(m_field, m_boolValue) );

				case INT:
					return std::unique_ptr<IFieldValue>( new FieldValue(m_field, m_intValue) );

				case DOUBLE:
					return std::unique_ptr<IFieldValue>( new FieldValue(m_field, m_doubleValue) );

				case STRING:
					return std::unique_ptr<IFieldValue>( new FieldValue(m_field, m_stringValue) );

				case DATETIME:
					return std::unique_ptr<IFieldValue>( new FieldValue(m_field, m_dateTimeValue) );

				case BINARY:
					throw std::runtime_error( "Binary field type not implemented." );

				default:
					throw std::runtime_error( "Invalid field type." );
					break;
			}
		}
	}

}}}
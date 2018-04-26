#include "stdafx.h"
#include "StubPrimaryKeyValue.h"

#include "StubField.h"
#include "StubFieldValue.h"


namespace systelab { namespace test_utility { 

	StubPrimaryKeyValue::StubPrimaryKeyValue(const IPrimaryKey& primaryKey)
		:m_primaryKey(primaryKey)
	{
		unsigned int nPrimaryKeyFields = m_primaryKey.getFieldsCount();
		for (unsigned int i = 0; i < nPrimaryKeyFields; i++)
		{
			std::unique_ptr<IFieldValue> fieldValue;

			const IField& field = primaryKey.getField(i);
			if (field.hasNullDefaultValue())
			{
				fieldValue.reset( new StubFieldValue(field.getName()) );
			}
			else
			{
				FieldTypes fieldType = field.getType();
				switch(fieldType)
				{
					case db::BOOLEAN:
						fieldValue.reset( new StubFieldValue(field.getName(), field.getBooleanDefaultValue()) );
						break;

					case db::INT:
						fieldValue.reset( new StubFieldValue(field.getName(), field.getIntDefaultValue()) );
						break;

					case db::DOUBLE:
						fieldValue.reset( new StubFieldValue(field.getName(), field.getDoubleDefaultValue()) );
						break;

					case STRING:
						fieldValue.reset( new StubFieldValue(field.getName(), field.getStringDefaultValue()) );
						break;

					case DATETIME:
						fieldValue.reset( new StubFieldValue(field.getName(), field.getDateTimeDefaultValue()) );
						break;

					case BINARY:
						throw std::exception( "Binary fields can't belong to primary key." );
						break;

					default:
						throw std::exception( "Unknown field type." );
				}
			}

			m_fieldValues.push_back( std::move(fieldValue) );
		}
	}

	StubPrimaryKeyValue::~StubPrimaryKeyValue()
	{
	}

	ITable& StubPrimaryKeyValue::getTable() const
	{
		return m_primaryKey.getTable();
	}

	const IPrimaryKey& StubPrimaryKeyValue::getPrimaryKey() const
	{
		return m_primaryKey;
	}

	unsigned int StubPrimaryKeyValue::getFieldValuesCount() const
	{
		return m_fieldValues.size();
	}

	IFieldValue& StubPrimaryKeyValue::getFieldValue(unsigned int index) const
	{
		if (index < m_fieldValues.size())
		{
			return *(m_fieldValues[index]);
		}
		else
		{
			throw std::exception( "Invalid primary key field index" );
		}
	}

	IFieldValue& StubPrimaryKeyValue::getFieldValue(const std::string& fieldName) const
	{
		unsigned int nFields = m_fieldValues.size();
		for (unsigned int i = 0; i < nFields; i++)
		{
			if (m_fieldValues[i]->getField().getName() == fieldName)
			{
				return *(m_fieldValues[i]);
			}
		}

		throw std::exception( "The requested primary key field doesn't exist" );
	}

}}
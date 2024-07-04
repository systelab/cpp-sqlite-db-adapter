#include "stdafx.h"
#include "Record.h"

#include "DateTimeHelper.h"
#include "Field.h"
#include "FieldValue.h"

#include "DbAdapterInterface/IRecordSet.h"

#include <sqleet/sqleet.h>


namespace systelab { namespace db { namespace sqlite {

	Record::Record(IRecordSet& recordSet, sqlite3_stmt* statement)
	{
		unsigned int nFields = recordSet.getFieldsCount();
		for( unsigned int i = 0; i < nFields; i++ )
		{
			std::unique_ptr<IFieldValue> fieldValue;
			const IField& field = recordSet.getField(i);
			unsigned int fieldIndex = field.getIndex();
			if (sqlite3_column_type(statement, fieldIndex) == SQLITE_NULL)
			{
				fieldValue.reset( new FieldValue(field) );
			}
			else
			{
				FieldTypes fieldType = field.getType();
				switch(fieldType)
				{
					case BOOLEAN:
					{
						bool boolValue = (sqlite3_column_int(statement, fieldIndex) != 0);
						fieldValue.reset( new FieldValue(field, boolValue) );
					}
					break;

					case INT:
					{
						int intValue = sqlite3_column_int(statement, fieldIndex);
						fieldValue.reset( new FieldValue(field, intValue) );
					}
					break;

					case DOUBLE:
					{
						double doubleValue = sqlite3_column_double(statement, fieldIndex);
						fieldValue.reset( new FieldValue(field, doubleValue) );
					}
					break;

					case STRING:
					{
						const unsigned char* charValue = sqlite3_column_text(statement, fieldIndex);
						if (charValue)
						{
							std::string stringValue( (char*) charValue );
							fieldValue.reset( new FieldValue(field, stringValue) );
						}
						else
						{
							fieldValue.reset( new FieldValue(field, std::string("")) );
						}
					}
					break;

					case DATETIME:
					{
						std::string stringValue( (char*) sqlite3_column_text(statement, fieldIndex) );
						fieldValue.reset( new FieldValue(field, date_time::toDateTime(stringValue)) );
					}
					break;

					case BINARY:
					{
						// TO DO
					}

					default:
						throw std::runtime_error( "Unknown field type." );
				}
			}

			m_fieldValues.push_back( std::move(fieldValue) );
		}
	}

	Record::Record( std::vector< std::unique_ptr<IFieldValue> >& fieldValues )
	{
		unsigned int nFieldValues = (unsigned int) fieldValues.size();
		for( unsigned int i = 0; i < nFieldValues; i++ )
		{
			m_fieldValues.push_back( std::move(fieldValues[i]) );
		}
	}

	Record::~Record()
	{
	}

	unsigned int Record::getFieldValuesCount() const
	{
		return (unsigned int) m_fieldValues.size();
	}

	IFieldValue& Record::getFieldValue(unsigned int index) const
	{
		if (index < m_fieldValues.size())
		{
			return *(m_fieldValues[index].get());
		}
		else
		{
			throw std::runtime_error( "Invalid field value index" );
		}
	}

	IFieldValue& Record::getFieldValue(const std::string& fieldName) const
	{
		unsigned int nFields = (unsigned int) m_fieldValues.size();
		for (unsigned int i = 0; i < nFields; i++)
		{
			if (m_fieldValues[i]->getField().getName() == fieldName)
			{
				return *(m_fieldValues[i].get());
			}
		}

		throw std::runtime_error( "The requested field value doesn't exist" );
	}

	bool Record::hasFieldValue(const std::string& fieldName) const
	{
		unsigned int nFields = (unsigned int) m_fieldValues.size();
		for (unsigned int i = 0; i < nFields; i++)
		{
			if (m_fieldValues[i]->getField().getName() == fieldName)
			{
				return true;
			}
		}

		return false;
	}

}}}
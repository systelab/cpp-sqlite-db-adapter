#include "TableRecord.h"

#include "Field.h"
#include "FieldValue.h"

#include "DbAdapterInterface/ITableRecordSet.h"

#include <sqlite3.h>


namespace systelab { namespace db { namespace sqlite {

	TableRecord::TableRecord(ITableRecordSet& recordSet, sqlite3_stmt* statement)
		:m_table(recordSet.getTable())
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
						fieldValue.reset( new FieldValue(field, boost::posix_time::from_iso_string(stringValue)) );
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

	TableRecord::TableRecord(ITable& table, std::vector< std::unique_ptr<IFieldValue> >& fieldValues)
		: m_table(table)
	{
		unsigned int nFieldValues = fieldValues.size();
		for( unsigned int i = 0; i < nFieldValues; i++ )
		{
			m_fieldValues.push_back( std::move(fieldValues[i]) );
		}
	}

	TableRecord::~TableRecord()
	{
	}

	ITable& TableRecord::getTable() const
	{
		return m_table;
	}

	unsigned int TableRecord::getFieldValuesCount() const
	{
		return m_fieldValues.size();
	}

	IFieldValue& TableRecord::getFieldValue(unsigned int index) const
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

	IFieldValue& TableRecord::getFieldValue(const std::string& fieldName) const
	{
		unsigned int nFields = m_fieldValues.size();
		for (unsigned int i = 0; i < nFields; i++)
		{
			std::string value = m_fieldValues[i]->getField().getName();
			if (m_fieldValues[i]->getField().getName() == fieldName)
			{
				return *(m_fieldValues[i].get());
			}
		}

		throw std::runtime_error( "The requested field value doesn't exist" );
	}

	bool TableRecord::hasFieldValue(const std::string& fieldName) const
	{
		unsigned int nFields = m_fieldValues.size();
		for (unsigned int i = 0; i < nFields; i++)
		{
			std::string value = m_fieldValues[i]->getField().getName();
			if (m_fieldValues[i]->getField().getName() == fieldName)
			{
				return true;
			}
		}

		return false;
	}

	std::vector<IFieldValue*> TableRecord::getValuesList() const
	{
		std::vector<IFieldValue*> values;
		unsigned int nRecordFieldValues = getFieldValuesCount();
		for(unsigned int i = 0; i < nRecordFieldValues; i++)
		{
			IFieldValue& recordFieldValue = getFieldValue(i);
			const IField& recordField = recordFieldValue.getField();
			if (!recordField.isPrimaryKey())
			{
				values.push_back(&recordFieldValue);
			}
		}
		return values;
	}
}}}
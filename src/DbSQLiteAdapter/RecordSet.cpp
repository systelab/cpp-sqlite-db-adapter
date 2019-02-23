#include "stdafx.h"
#include "RecordSet.h"

#include "Field.h"
#include "Record.h"

#include <sqlite/sqlite3.h>


namespace systelab { namespace db { namespace sqlite {

	RecordSet::RecordSet(sqlite3_stmt* statement, bool allFieldsAsStrings)
	{
		bool hasNext = (sqlite3_step(statement) == SQLITE_ROW);
		createFields(statement, allFieldsAsStrings);

		while (hasNext)
		{
			m_records.push_back( std::unique_ptr<IRecord>(new Record(*this, statement)) );
			hasNext = (sqlite3_step(statement) == SQLITE_ROW);
		}

		sqlite3_finalize(statement);

		m_iterator = m_records.begin();
	}

	RecordSet::~RecordSet()
	{

	}

	unsigned int RecordSet::getFieldsCount() const
	{
		return m_fields.size();
	}

	const IField& RecordSet::getField(unsigned int index) const
	{
		if (index < m_fields.size())
		{
			return *(m_fields[index].get());
		}
		else
		{
			throw std::runtime_error( "Invalid field index" );
		}
	}

	const IField& RecordSet::getField(const std::string& fieldName) const
	{
		unsigned int nFields = m_fields.size();
		for (unsigned int i = 0; i < nFields; i++)
		{
			if (m_fields[i]->getName() == fieldName)
			{
				return *(m_fields[i].get());
			}
		}

		throw std::runtime_error( "The requested field doesn't exist" );
	}

	unsigned int RecordSet::getRecordsCount() const
	{
		return m_records.size();
	}

	const IRecord& RecordSet::getCurrentRecord() const
	{
		return *m_iterator->get();
	}

	std::unique_ptr<IRecord> RecordSet::copyCurrentRecord() const
	{
		const IRecord& currentRecord = getCurrentRecord();

		std::vector< std::unique_ptr<IFieldValue> > copiedFieldValues;
		unsigned int nFieldValues = currentRecord.getFieldValuesCount();
		for (unsigned int i = 0; i < nFieldValues; i++)
		{
			IFieldValue& fieldValue = currentRecord.getFieldValue(i);
			copiedFieldValues.push_back( fieldValue.clone() );
		}

		return std::unique_ptr<IRecord>( new Record(copiedFieldValues) );
	}

	bool RecordSet::isCurrentRecordValid() const
	{
		return (m_iterator != m_records.end());
	}

	void RecordSet::nextRecord()
	{
		m_iterator++;
	}

	void RecordSet::createFields(sqlite3_stmt* statement, bool allFieldsAsStrings)
	{
		int nFields = sqlite3_column_count(statement);
		for( int i = 0; i < nFields; i++ )
		{
			std::string fieldName( sqlite3_column_name(statement, i) );
			int sqliteFieldType = allFieldsAsStrings ? SQLITE_TEXT : sqlite3_column_type(statement, i);

			FieldTypes fieldType = getTypeFromSQLiteType(sqliteFieldType);
			std::unique_ptr<IField> field( new Field(i, fieldName, fieldType, "", false) );
			m_fields.push_back( std::move(field) );
		}
	}

	FieldTypes RecordSet::getTypeFromSQLiteType(int SQLiteType)
	{
		switch(SQLiteType)
		{
			case SQLITE_INTEGER:
				return INT;

			case SQLITE_FLOAT:
				return DOUBLE;

			case SQLITE_NULL:
			case SQLITE_TEXT:
				return STRING;

			case SQLITE_BLOB:
				return BINARY;

			// Not implemented 
				//return DATETIME;

			default:
				throw std::runtime_error("SQLite type unsupported: " + SQLiteType );
		}
	}

}}}
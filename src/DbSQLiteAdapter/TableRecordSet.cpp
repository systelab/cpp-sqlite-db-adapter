#include "TableRecordSet.h"

#include "Field.h"
#include "TableRecord.h"

#include "DbAdapterInterface/ITable.h"

#include <sqlite/sqlite3.h>


namespace systelab { namespace db { namespace sqlite {

	TableRecordSet::TableRecordSet(ITable& table, sqlite3_stmt* statement)
		:m_table(table)
	{
		while (sqlite3_step(statement) == SQLITE_ROW)
		{
			m_records.push_back( std::unique_ptr<ITableRecord>(new TableRecord(*this, statement)) );
		}

		sqlite3_finalize(statement);

		m_iterator = m_records.begin();
	}

	TableRecordSet::~TableRecordSet()
	{
	}

	ITable& TableRecordSet::getTable() const
	{
		return m_table;
	}

	unsigned int TableRecordSet::getFieldsCount() const
	{
		return m_table.getFieldsCount();
	}

	const IField& TableRecordSet::getField(unsigned int index) const
	{
		return m_table.getField(index);
	}

	const IField& TableRecordSet::getField(const std::string& fieldName) const
	{
		return m_table.getField(fieldName);
	}

	unsigned int TableRecordSet::getRecordsCount() const
	{
		return m_records.size();
	}

	const ITableRecord& TableRecordSet::getCurrentRecord() const
	{
		return *m_iterator->get();
	}

	std::unique_ptr<ITableRecord> TableRecordSet::copyCurrentRecord() const
	{
		const ITableRecord& currentRecord = getCurrentRecord();

		std::vector< std::unique_ptr<IFieldValue> > copiedFieldValues;
		unsigned int nFieldValues = currentRecord.getFieldValuesCount();
		for (unsigned int i = 0; i < nFieldValues; i++)
		{
			IFieldValue& fieldValue = currentRecord.getFieldValue(i);
			copiedFieldValues.push_back( fieldValue.clone() );
		}

		return std::unique_ptr<ITableRecord>( new TableRecord(m_table, copiedFieldValues) );
	}

	bool TableRecordSet::isCurrentRecordValid() const
	{
		return (m_iterator != m_records.end());
	}

	void TableRecordSet::nextRecord()
	{
		m_iterator++;
	}

}}}
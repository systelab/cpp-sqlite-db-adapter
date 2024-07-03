#pragma once

#include "DbAdapterInterface/ITableRecord.h"

#include <memory>
#include <vector>


struct sqlite3_stmt;

namespace systelab { namespace db {
	class ITableRecordSet;
}}

namespace systelab { namespace db { namespace sqlite {

	class TableRecord : public ITableRecord
	{
	public:
		TableRecord(ITableRecordSet& recordSet, sqlite3_stmt* statement);
		TableRecord(ITable&, std::vector<std::unique_ptr<IFieldValue>>&);
		~TableRecord();

		ITable& getTable() const override;

		unsigned int getFieldValuesCount() const override;
		IFieldValue& getFieldValue(unsigned int index) const override;
		IFieldValue& getFieldValue(const std::string& fieldName) const override;

		bool hasFieldValue(const std::string& fieldName) const override;

		std::vector<IFieldValue*> getValuesList() const override;

	private:
		ITable& m_table;
		std::vector<std::unique_ptr<IFieldValue>> m_fieldValues;
	};

}}}

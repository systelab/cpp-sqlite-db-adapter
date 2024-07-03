#pragma once

#include "DbAdapterInterface/IRecordSet.h"
#include "DbAdapterInterface/Types.h"

#include <memory>
#include <vector>

struct sqlite3_stmt;

namespace systelab { namespace db {
	class IField;
}}

namespace systelab { namespace db { namespace sqlite {

	class RecordSet : public IRecordSet
	{
	public:
		RecordSet(sqlite3_stmt* statement, bool allFieldsAsStrings);
		~RecordSet();

		unsigned int getFieldsCount() const override;
		const IField& getField(unsigned int index) const override;
		const IField& getField(const std::string& fieldName) const override;

		unsigned int getRecordsCount() const override;

		const IRecord& getCurrentRecord() const override;
		std::unique_ptr<IRecord> copyCurrentRecord() const override;
		bool isCurrentRecordValid() const override;
		void nextRecord() override;

	private:
		std::vector<std::unique_ptr<IField>> m_fields;
		std::vector<std::unique_ptr<IRecord>> m_records;
		std::vector<std::unique_ptr<IRecord>>::iterator m_iterator;

		void createFields(sqlite3_stmt* statement, bool allFieldsAsStrings);
		FieldTypes getTypeFromSQLiteType(int SQLiteType);
	};

}}}

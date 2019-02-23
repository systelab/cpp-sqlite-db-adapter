#ifndef _DBSQLITEADAPTER_RECORDSET_QUIM_VILA_21212151651_H
#define _DBSQLITEADAPTER_RECORDSET_QUIM_VILA_21212151651_H

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
		virtual ~RecordSet();

		unsigned int getFieldsCount() const;
		const IField& getField(unsigned int index) const;
		const IField& getField(const std::string& fieldName) const;

		unsigned int getRecordsCount() const;

		const IRecord& getCurrentRecord() const;
		std::unique_ptr<IRecord> copyCurrentRecord() const;
		bool isCurrentRecordValid() const;
		void nextRecord();

	private:
		std::vector< std::unique_ptr<IField> > m_fields;
		std::vector< std::unique_ptr<IRecord> > m_records;
		std::vector< std::unique_ptr<IRecord> >::iterator m_iterator;

		void createFields(sqlite3_stmt* statement, bool allFieldsAsStrings);
		FieldTypes getTypeFromSQLiteType(int SQLiteType);
	};

}}}

#endif //_DBSQLITEADAPTER_RECORDSET_QUIM_VILA_21212151651_H
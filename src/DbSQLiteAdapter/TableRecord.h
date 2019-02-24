#ifndef _DBSQLITEADAPTER_TABLERECORD_QV_0303161116_H
#define _DBSQLITEADAPTER_TABLERECORD_QV_0303161116_H

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
		TableRecord(ITable&, std::vector< std::unique_ptr<IFieldValue> >& );
		virtual ~TableRecord();

		ITable& getTable() const;

		unsigned int getFieldValuesCount() const;
		IFieldValue& getFieldValue(unsigned int index) const;
		IFieldValue& getFieldValue(const std::string& fieldName) const;

		bool hasFieldValue(const std::string& fieldName) const;

		std::vector<IFieldValue*> getValuesList() const;

	private:
		ITable& m_table;
		std::vector< std::unique_ptr<IFieldValue> > m_fieldValues;
	};

}}}

#endif //_DBSQLITEADAPTER_TABLERECORD_QV_0303161116_H
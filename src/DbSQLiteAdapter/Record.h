#ifndef _DBSQLITEADAPTER_RECORD_QUIM_VILA_2112151735_H
#define _DBSQLITEADAPTER_RECORD_QUIM_VILA_2112151735_H

#include "DbAdapterInterface/IRecord.h"

#include <memory>
#include <vector>


struct sqlite3_stmt;

namespace systelab { namespace db {
	class IRecordSet;
}}

namespace systelab { namespace db { namespace sqlite {

	class Record : public IRecord
	{
	public:
		Record(IRecordSet& recordSet, sqlite3_stmt* statement);
		Record( std::vector< std::unique_ptr<IFieldValue> >& );
		virtual ~Record();

		unsigned int getFieldValuesCount() const;
		IFieldValue& getFieldValue(unsigned int index) const;
		IFieldValue& getFieldValue(const std::string& fieldName) const;

		bool hasFieldValue(const std::string& fieldName) const;

	private:
		std::vector< std::unique_ptr<IFieldValue> > m_fieldValues;

	};

}}}

#endif //_DBSQLITEADAPTER_RECORD_QUIM_VILA_2112151735_H
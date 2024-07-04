#pragma once

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
		explicit Record(std::vector<std::unique_ptr<IFieldValue>>&);
		~Record();

		unsigned int getFieldValuesCount() const override;
		IFieldValue& getFieldValue(unsigned int index) const override;
		IFieldValue& getFieldValue(const std::string& fieldName) const override;

		bool hasFieldValue(const std::string& fieldName) const override;

	private:
		std::vector< std::unique_ptr<IFieldValue> > m_fieldValues;
	};

}}}

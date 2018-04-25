#ifndef _DBADAPTERINTERFACE_IDATABASE_JOSEP_VILA_1712151006_H
#define _DBADAPTERINTERFACE_IDATABASE_JOSEP_VILA_1712151006_H

#include "Types.h"
#include "ITable.h"
#include "IRecordSet.h"
#include "ITransaction.h"
#include <memory>

namespace systelab { namespace db {
	class IDatabase
	{
	public:
		virtual ~IDatabase() {}

		virtual ITable& getTable(std::string tableName) = 0;
		virtual std::unique_ptr<IRecordSet> executeQuery(const std::string& query) = 0;
		virtual void executeOperation(const std::string& operation) = 0;
		virtual void executeMultipleStatements(const std::string& statements) = 0;
		virtual RowsAffected getRowsAffectedByLastChangeOperation() const = 0;
		virtual RowId getLastInsertedRowId() const = 0;
		virtual std::unique_ptr<ITransaction> startTransaction() = 0;
	};
}}

#endif //_DBADAPTERINTERFACE_IDATABASE_JOSEP_VILA_1712151006_H
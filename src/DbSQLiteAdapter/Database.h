#pragma once

#include "DbAdapterInterface/IDatabase.h"
#include "DbAdapterInterface/ITableRecordSet.h"

#include <map>
#include <mutex>

struct sqlite3;

namespace systelab::db::sqlite 
{
	class Database : public IDatabase
	{
	public:
		explicit Database(sqlite3* database, const std::string& filepath);
		~Database();

		struct Lock : public std::lock_guard<std::recursive_mutex>
		{
			Lock(Database&);
		};

		ITable& getTable(const std::string& tableName) override;
		std::unique_ptr<IRecordSet> executeQuery(const std::string& query) override;
		
		void executeOperation(const std::string& operation) override;
		void executeMultipleStatements(const std::string& statements) override;
		RowsAffected getRowsAffectedByLastChangeOperation() const override;
		RowId getLastInsertedRowId() const override;
		std::unique_ptr<ITransaction> startTransaction() override;

		// Internal usage
		std::unique_ptr<IRecordSet> executeQuery(const std::string& query, bool allFieldsAsStrings);
		std::unique_ptr<ITableRecordSet> executeTableQuery(const std::string& query, ITable& table);

	private:
		sqlite3* m_database;
		std::string m_filepath;

		std::map< std::string, std::unique_ptr<ITable> > m_tables;
		std::recursive_mutex m_mutex;
	};
}
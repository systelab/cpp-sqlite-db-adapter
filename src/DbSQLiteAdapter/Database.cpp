#include "stdafx.h"
#include "Database.h"

#include "Table.h"
#include "Transaction.h"
#include "RecordSet.h"
#include "TableRecordSet.h"

#include <sqlite/sqlite3.h>


namespace systelab { namespace db { namespace sqlite {

	Database::Database(sqlite3* database)
		:m_database(database)
	{
	}

	Database::~Database()
	{
		sqlite3_close(m_database);
	}

	Database::Lock::Lock(Database& db)
		:boost::unique_lock<boost::mutex>(db.m_mutex)
	{
	}

	ITable& Database::getTable(std::string tableName)
	{
		std::map< std::string, std::unique_ptr<ITable> >::iterator itr = m_tables.find(tableName);
		if (itr == m_tables.end())
		{
			m_tables.insert( std::make_pair(tableName, new Table(*this, tableName)) );
		}

		ITable* table = m_tables[ tableName ].get();
		return *table;
	}

	std::unique_ptr<IRecordSet> Database::executeQuery(const std::string& query)
	{
		return executeQuery(query, false);
	}

	std::unique_ptr<IRecordSet> Database::executeQuery(const std::string& query, bool allFieldsAsStrings)
	{
		sqlite3_stmt* statement = 0;
		if(sqlite3_prepare_v2(m_database, query.c_str(), -1, &statement, 0) == SQLITE_OK)
		{
			return std::unique_ptr<IRecordSet>( new RecordSet(statement, allFieldsAsStrings) );
		}
		else
		{
			std::string error = sqlite3_errmsg(m_database);
			std::cout << "# ERR: SQLException in " << __FILE__;
			std::cout << "(Database::executeQuery) on line " << __LINE__ << std::endl;
			std::cout << "# ERR: " << error << std::endl;

			return std::unique_ptr<IRecordSet>();
		}
	}

	std::unique_ptr<ITableRecordSet> Database::executeTableQuery(const std::string& query, ITable& table)
	{
		sqlite3_stmt* statement = 0;
		if(sqlite3_prepare_v2(m_database, query.c_str(), -1, &statement, 0) == SQLITE_OK)
		{
			return std::unique_ptr<ITableRecordSet>( new TableRecordSet(table, statement) );
		}
		else
		{
			std::string error = sqlite3_errmsg(m_database);
			std::cout << "# ERR: SQLException in " << __FILE__;
			std::cout << "(Database::executeQuery) on line " << __LINE__ << std::endl;
			std::cout << "# ERR: " << error << std::endl;

			return std::unique_ptr<ITableRecordSet>();
		}
	}

	void Database::executeOperation(const std::string& operation)
	{
		sqlite3_stmt* statement = 0;
		if(sqlite3_prepare_v2(m_database, operation.c_str(), -1, &statement, 0) == SQLITE_OK)
		{
			int res = sqlite3_step(statement);
			sqlite3_finalize(statement);
			if (res != SQLITE_DONE)
			{
				std::ostringstream exceptionStream;
				std::string sqlError = sqlite3_errmsg(m_database);
				exceptionStream << "# ERR: SQLException in " << __FILE__;
				exceptionStream << "(Database::executeQuery) on line " << __LINE__ << std::endl;
				exceptionStream << "# ERR: " << sqlError << std::endl;

				std::string exceptionStr = exceptionStream.str();
				throw std::runtime_error(exceptionStr.c_str());
			}
		}
		else
		{
			std::ostringstream exceptionStream;
			std::string sqlError = sqlite3_errmsg(m_database);
			exceptionStream << "# ERR: SQLException in " << __FILE__;
			exceptionStream << "(Database::executeQuery) on line " << __LINE__ << std::endl;
			exceptionStream << "# ERR: " << sqlError << std::endl;

			std::string exceptionStr = exceptionStream.str();
			throw std::runtime_error(exceptionStr.c_str());
		}
	}

	void Database::executeMultipleStatements(const std::string& statements)
	{
		char* errors = NULL;
		if(sqlite3_exec(m_database, statements.c_str(), NULL, NULL, &errors) != SQLITE_OK)
		{
			std::ostringstream exceptionStream;
			std::string sqlError = sqlite3_errmsg(m_database);
			std::string sqlError2;
			if (errors != NULL)
			{
				sqlError2 = errors;
				sqlite3_free(errors);
			}
			exceptionStream << "# ERR: SQLException in " << __FILE__;
			exceptionStream << "(Database::executeQuery) on line " << __LINE__ << std::endl;
			exceptionStream << "# ERR: " << sqlError << std::endl;

			std::string exceptionStr = exceptionStream.str();
			throw std::runtime_error(exceptionStr.c_str());
		}
		if (errors != NULL)
		{
			std::string sqlError2;
			sqlError2 = errors;
			sqlite3_free(errors);
		}
	}

	RowsAffected Database::getRowsAffectedByLastChangeOperation() const
	{
		return sqlite3_changes(m_database);
	}

	RowId Database::getLastInsertedRowId() const
	{
		return (RowId) sqlite3_last_insert_rowid(m_database);
	}

	std::unique_ptr<ITransaction> Database::startTransaction()
	{
		std::string operation = "BEGIN TRANSACTION";
		executeOperation(operation);
		return std::unique_ptr<ITransaction>( new Transaction(*this) );
	}

}}}
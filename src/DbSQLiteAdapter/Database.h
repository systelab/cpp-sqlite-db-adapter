#ifndef _DBSQLITEADAPTER_DATABASE_QUIM_VILA_2112151554_H
#define _DBSQLITEADAPTER_DATABASE_QUIM_VILA_2112151554_H

#include "DbAdapterInterface/IDatabase.h"
#include "DbAdapterInterface/ITableRecordSet.h"

#include <map>
#include <boost/thread/recursive_mutex.hpp>


struct sqlite3;

namespace systelab { namespace db { namespace sqlite {

	class Database : public IDatabase
	{
	public:
		Database(sqlite3* database);
		virtual ~Database();

		struct Lock : public boost::unique_lock<boost::recursive_mutex>
		{
			Lock(Database&);
		};

		ITable& getTable(std::string tableName);
		std::unique_ptr<IRecordSet> executeQuery(const std::string& query);
		std::unique_ptr<IRecordSet> executeQuery(const std::string& query, bool allFieldsAsStrings);
		std::unique_ptr<ITableRecordSet> executeTableQuery(const std::string& query, ITable& table);
		void executeOperation(const std::string& operation);
		void executeMultipleStatements(const std::string& statements);
		RowsAffected getRowsAffectedByLastChangeOperation() const;
		RowId getLastInsertedRowId() const;
		std::unique_ptr<ITransaction> startTransaction();

	private:
		sqlite3* m_database;
		std::map< std::string, std::unique_ptr<ITable> > m_tables;
		boost::recursive_mutex m_mutex;
	};

}}}

#endif //_DBSQLITEADAPTER_DATABASE_QUIM_VILA_2112151554_H
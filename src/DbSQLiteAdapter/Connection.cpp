#include "stdafx.h"
#include "Connection.h"
#include "Database.h"

#include <DbAdapterInterface/IConnectionConfiguration.h>

#include <excpt.h>
#include <sqleet/sqleet.h>

namespace systelab::db::sqlite 
{
	Connection::Connection() = default;
	Connection::~Connection() = default;

	std::unique_ptr<IDatabase> Connection::loadDatabase(IConnectionConfiguration& configuration)
	{
		std::string filepath = configuration.getParameter("filepath");
		sqlite3* database = nullptr;

		int openStatusCode = sqlite3_open(filepath.c_str(), &database);
		if (openStatusCode != SQLITE_OK)
		{
			std::string extendedMessage = sqlite3_errmsg(database);
			int extendedErrorCode = sqlite3_extended_errcode(database);
			throw SQLiteException("Unable to open database file '" + filepath + "'", extendedMessage, openStatusCode, extendedErrorCode);
		}

		if (configuration.hasParameter("key"))
		{
			std::string key = configuration.getParameter("key");
			int keyStatusCode = sqlite3_key(database, key.c_str(), (int) key.size());
			if (keyStatusCode != SQLITE_OK)
			{
				std::string extendedMessage = sqlite3_errmsg(database);
				int extendedErrorCode = sqlite3_extended_errcode(database);
				sqlite3_close(database);
				throw SQLiteException("Invalid encryption key", extendedMessage, keyStatusCode, extendedErrorCode);
			}
		}

		auto db = std::make_unique<Database>(database);
		db->executeOperation("PRAGMA foreign_keys = ON");

		return db;
	}

}

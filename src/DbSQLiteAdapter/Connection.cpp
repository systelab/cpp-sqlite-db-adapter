#include "stdafx.h"
#include "Connection.h"
#include "Database.h"

#include <sqlite/sqlite3.h>


namespace systelab { namespace db { namespace sqlite {

	Connection::Connection()
	{
	}

	Connection::~Connection()
	{
	}

	std::unique_ptr<IDatabase> Connection::loadDatabase(IConnectionConfiguration& configuration)
	{
		sqlite3* database;
		std::string filepath = configuration.getParameter("filepath");
		if ( sqlite3_open(filepath.c_str(), &database) == SQLITE_OK )
		{
			std::unique_ptr<IDatabase> db( new Database(database) );
			db->executeOperation("PRAGMA foreign_keys = ON");
			return db;
		}
		else
		{
			return std::unique_ptr<IDatabase>();
		}
	}

}}}
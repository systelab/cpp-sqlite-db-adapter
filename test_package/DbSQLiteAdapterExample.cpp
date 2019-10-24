#define _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING 1

#include <iostream>
#include <stdexcept>

#include "DbSQLiteAdapter/Connection.h"
#include "DbSQLiteAdapter/ConnectionConfiguration.h"

#include "DbAdapterInterface/IDatabase.h"


int main(int argc, char *argv[])
{
	systelab::db::sqlite::ConnectionConfiguration configuration("./testdb.db");
	systelab::db::sqlite::Connection connection;
	std::unique_ptr<systelab::db::IDatabase> database = connection.loadDatabase(configuration);
	
	std::cout << "Database connection established successfully" << std::endl;

    return 0;
}

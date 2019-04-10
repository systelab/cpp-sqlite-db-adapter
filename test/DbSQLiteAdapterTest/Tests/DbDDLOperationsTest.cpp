#include "stdafx.h"

#include "Helpers/Helpers.h"
#include "DbSQLiteAdapter/ConnectionConfiguration.h"

#include "DbAdapterInterface/IConnection.h"
#include "DbSQLiteAdapter/Connection.h"

#include <boost/filesystem.hpp>

using namespace testing;

namespace systelab { namespace db { namespace sqlite { namespace unit_test {
		
	static const std::string DDL_DATABASE_FILEPATH = "ddl-test.db";
	static const std::string MAIN_TABLE = "MAIN_TABLE";
	static const std::string DUMMY_TABLE = "DUMMY_TABLE";

	class DbDDLOperationsTest: public testing::Test
	{
	public:
		void SetUp()
		{
			if (boost::filesystem::exists(DDL_DATABASE_FILEPATH))
			{
				boost::filesystem::remove(DDL_DATABASE_FILEPATH);
			}

			systelab::db::sqlite::ConnectionConfiguration connectionConfiguration(DDL_DATABASE_FILEPATH, "keyForTest1234"s);
			m_db = m_connection.loadDatabase(connectionConfiguration);

			IDatabase& db = *(m_db.get());
			dropTable(db, MAIN_TABLE);
			dropTable(db, DUMMY_TABLE);
			createTable(db, MAIN_TABLE, 100);
		}

	public:
		std::unique_ptr<IDatabase> m_db;
		systelab::db::sqlite::Connection m_connection;
	};


	// INTENDED USE 1: Execution of DDL operations
	
	// Create Table
	TEST_F(DbDDLOperationsTest, testDDLCreateTable)
	{
		// Check there is no table at the database with the given name:
		std::unique_ptr<IRecordSet> recordSet = m_db->executeQuery("SELECT name FROM sqlite_master WHERE type='table' AND name='"+DUMMY_TABLE+"';");
		ASSERT_EQ(0, recordSet->getRecordsCount());

		// Creates an empty table and checks if the table exists within the DB afterwards.
		m_db->executeOperation("CREATE TABLE "+DUMMY_TABLE+" (ID INT PRIMARY KEY NOT NULL, FIELD_INT_INDEX INT, FIELD_INT_NO_INDEX INT, FIELD_STR_INDEX CHAR(255), FIELD_STR_NO_INDEX CHAR(255), FIELD_DATE DATETIME DEFAULT '2016-05-05')");	
		recordSet = m_db->executeQuery("SELECT name FROM sqlite_master WHERE type='table' AND name='"+DUMMY_TABLE+"';");
		ASSERT_EQ(1, recordSet->getRecordsCount());
	}

	TEST_F(DbDDLOperationsTest, testDDLCreateInvalidTable)
	{
		std::unique_ptr<IRecordSet> recordSet = m_db->executeQuery("SELECT name FROM sqlite_master WHERE type='table' AND name='"+MAIN_TABLE+"';");
		ASSERT_EQ(1, recordSet->getRecordsCount());

		// Attempts to create a table tha already exists in the database.
		ASSERT_THROW(m_db->executeOperation("CREATE TABLE "+MAIN_TABLE+" (ID INT PRIMARY KEY NOT NULL, FIELD_INT_INDEX INT, FIELD_INT_NO_INDEX INT, FIELD_STR_INDEX CHAR(255), FIELD_STR_NO_INDEX CHAR(255), FIELD_DATE DATETIME DEFAULT '2016-05-05')"), std::exception);	
		recordSet = m_db->executeQuery("SELECT name FROM sqlite_master WHERE type='table' AND name='"+MAIN_TABLE+"';");
		ASSERT_EQ(1, recordSet->getRecordsCount());
	}

	// Drop Table
	TEST_F(DbDDLOperationsTest, testDDLDropTable)
	{
		// Creates an empty table and checks if the table exists within the DB afterwards.
		m_db->executeOperation("CREATE TABLE "+DUMMY_TABLE+" (ID INT PRIMARY KEY NOT NULL, FIELD_INT_INDEX INT, FIELD_INT_NO_INDEX INT, FIELD_STR_INDEX CHAR(255), FIELD_STR_NO_INDEX CHAR(255), FIELD_DATE DATETIME DEFAULT '2016-05-05')");	
		std::unique_ptr<IRecordSet> recordSet = m_db->executeQuery("SELECT name FROM sqlite_master WHERE type='table' AND name='"+DUMMY_TABLE+"';");
		ASSERT_EQ(1, recordSet->getRecordsCount());

		IDatabase& db = *(m_db.get());
		m_db->executeOperation("DROP TABLE "+DUMMY_TABLE);	

		recordSet = m_db->executeQuery("SELECT name FROM sqlite_master WHERE type='table' AND name='"+DUMMY_TABLE+"';");
		ASSERT_EQ(0, recordSet->getRecordsCount());
	}

	TEST_F(DbDDLOperationsTest, testDDLDropInvalidTable)
	{
		std::unique_ptr<IRecordSet> recordSet = m_db->executeQuery("SELECT name FROM sqlite_master WHERE type='table' AND name='"+DUMMY_TABLE+"';");
		ASSERT_EQ(0, recordSet->getRecordsCount());

		// Attempts to drop a non-existing table from the database.
		IDatabase& db = *(m_db.get());
		ASSERT_THROW(m_db->executeOperation("DROP TABLE "+DUMMY_TABLE), std::exception);

		recordSet = m_db->executeQuery("SELECT name FROM sqlite_master WHERE type='table' AND name='"+DUMMY_TABLE+"';");
		ASSERT_EQ(0, recordSet->getRecordsCount());
	}


	// Create index
	TEST_F(DbDDLOperationsTest, testDDLCreateValidIndex)
	{
		std::string indexName = "INT_INDEX_2";
		std::string indexFieldName = "FIELD_INT_INDEX";

		// Creates an empty table and checks if the table exists within the DB afterwards.
		m_db->executeOperation("CREATE TABLE "+DUMMY_TABLE+" (ID INT PRIMARY KEY NOT NULL, "+indexFieldName+" INT, FIELD_INT_NO_INDEX INT, FIELD_STR_INDEX CHAR(255), FIELD_STR_NO_INDEX CHAR(255), FIELD_DATE DATETIME DEFAULT '2016-05-05')");	
		std::unique_ptr<IRecordSet> recordSet = m_db->executeQuery("SELECT name FROM sqlite_master WHERE type == 'index' AND tbl_name == '"+DUMMY_TABLE+"'");

		// Check we have 1 indexes at the table before creating the index (the primary key generates an index):
		ASSERT_EQ(1, recordSet->getRecordsCount());

		// Create the index, whose name has to be unique in the whole database.
		m_db->executeOperation("CREATE INDEX "+indexName+" ON "+DUMMY_TABLE+"("+indexFieldName+")"); 

		// Check we have 2 indexes at the table after creating the index:
		recordSet = m_db->executeQuery("SELECT name FROM sqlite_master WHERE type == 'index' AND tbl_name == '"+DUMMY_TABLE+"'");
		ASSERT_EQ(2, recordSet->getRecordsCount());
	}

	TEST_F(DbDDLOperationsTest, testDDLCreateInvalidIndex)
	{
		std::string indexName = "INT_INDEX_" + MAIN_TABLE;
		std::string indexFieldName = "FIELD_INT_INDEX";
		// Creates an empty table and checks if the table exists within the DB afterwards.
		m_db->executeOperation("CREATE TABLE "+DUMMY_TABLE+" (ID INT PRIMARY KEY NOT NULL, "+indexFieldName+" INT, FIELD_INT_NO_INDEX INT, FIELD_STR_INDEX CHAR(255), FIELD_STR_NO_INDEX CHAR(255), FIELD_DATE DATETIME DEFAULT '2016-05-05')");	
		std::unique_ptr<IRecordSet> recordSet = m_db->executeQuery("SELECT name FROM sqlite_master WHERE type == 'index' AND tbl_name == '"+DUMMY_TABLE+"'");

		// Check we have 1 indexes at the table before creating the index (the primary key generates an index):
		ASSERT_EQ(1, recordSet->getRecordsCount());

		// Create an index already existent in current DB (in MAIN_TABLE)
		ASSERT_THROW(m_db->executeOperation("CREATE INDEX "+indexName+" ON "+DUMMY_TABLE+"("+indexFieldName+")"), std::exception); // Create the index, whose name has to be unique in the whole database.

		// Check we still have 1 index after attempting to create the index:
		recordSet = m_db->executeQuery("SELECT name FROM sqlite_master WHERE type == 'index' AND tbl_name == '"+DUMMY_TABLE+"'");
		ASSERT_EQ(1, recordSet->getRecordsCount());
	}

	// Drop index
	TEST_F(DbDDLOperationsTest, testDDLDropValidIndex)
	{
		std::string indexName = "INT_INDEX_" + MAIN_TABLE;

		// Check that we have 3 indexes (primary key + 2 created indexes) at MAIN_TABLE.
		std::unique_ptr<IRecordSet> recordSet = m_db->executeQuery("SELECT name FROM sqlite_master WHERE type == 'index' AND tbl_name == '"+MAIN_TABLE+"'");

		ASSERT_EQ(3, recordSet->getRecordsCount());

		// Drop an index from the table
		m_db->executeOperation("DROP INDEX "+indexName); 

		recordSet = m_db->executeQuery("SELECT name FROM sqlite_master WHERE type == 'index' AND tbl_name == '"+MAIN_TABLE+"'");

		ASSERT_EQ(2, recordSet->getRecordsCount());
	}

	TEST_F(DbDDLOperationsTest, testDDLDropInvalidIndex)
	{
		std::string indexName = "INT_INDEX24";

		// Check that we have 3 indexes (primary key + 2 created indexes) at MAIN_TABLE.
		std::unique_ptr<IRecordSet> recordSet = m_db->executeQuery("SELECT name FROM sqlite_master WHERE type == 'index' AND tbl_name == '"+MAIN_TABLE+"'");

		ASSERT_EQ(3, recordSet->getRecordsCount());

		// Drop an index from the table
		ASSERT_THROW(m_db->executeOperation("DROP INDEX "+indexName),std::exception);

		recordSet = m_db->executeQuery("SELECT name FROM sqlite_master WHERE type == 'index' AND tbl_name == '"+MAIN_TABLE+"'");

		ASSERT_EQ(3, recordSet->getRecordsCount());
	}
}}}}
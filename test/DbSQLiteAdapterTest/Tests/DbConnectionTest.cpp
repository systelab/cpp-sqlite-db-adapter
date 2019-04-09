#include "stdafx.h"
#include "Helpers/Helpers.h"

#include "DbAdapterInterface/IConnection.h"
#include "DbSQLiteAdapter/Connection.h"
#include "DbSQLiteAdapter/ConnectionConfiguration.h"

#include <boost/filesystem.hpp>

using namespace testing;

namespace systelab { namespace db { namespace sqlite { namespace unit_test {

	class DbConnectionTest : public testing::Test
	{
	public:
		void SetUp()
		{
			m_dbFilePath = boost::filesystem::absolute(boost::filesystem::path(std::string("DbConnectionTest") / boost::filesystem::path("test.db")));
			boost::filesystem::path folderPath = m_dbFilePath.parent_path();
			if (!boost::filesystem::exists(folderPath))
			{
				boost::filesystem::create_directories(folderPath);
			}

			if (boost::filesystem::exists(m_dbFilePath))
			{
				boost::filesystem::remove(m_dbFilePath);
			}
		}

		void TearDown()
		{
			if (boost::filesystem::exists(m_dbFilePath))
			{
				boost::filesystem::remove(m_dbFilePath);
			}
		}

	protected:
		boost::filesystem::path m_dbFilePath;
	};


	// Happy path WITHOUT encryption
	TEST_F(DbConnectionTest, testLoadDatabaseForHappyPathWithoutEncryptionReturnsDatabaseObject)
	{
		systelab::db::sqlite::Connection connection;
		auto database = connection.loadDatabase(systelab::db::sqlite::ConnectionConfiguration(m_dbFilePath.string()));
		ASSERT_TRUE(database != NULL);
	}

	TEST_F(DbConnectionTest, testLoadDatabaseForHappyPathWithoutEncryptionCreatesDBFile)
	{
		systelab::db::sqlite::Connection connection;
		connection.loadDatabase(systelab::db::sqlite::ConnectionConfiguration(m_dbFilePath.string()));
		ASSERT_TRUE(boost::filesystem::exists(m_dbFilePath));
	}


	// Happy path WITH encryption
	TEST_F(DbConnectionTest, testLoadDatabaseForHappyPathWithEncryptionReturnsDatabaseObject)
	{
		systelab::db::sqlite::Connection connection;
		auto database = connection.loadDatabase(systelab::db::sqlite::ConnectionConfiguration(m_dbFilePath.string(), "MyEncryptionKey"s));
		ASSERT_TRUE(database != NULL);
	}

	TEST_F(DbConnectionTest, testLoadDatabaseForHappyPathWithEncryptionCreatesDBFile)
	{
		systelab::db::sqlite::Connection connection;
		connection.loadDatabase(systelab::db::sqlite::ConnectionConfiguration(m_dbFilePath.string(), "MyEncryptionKey"s));
		ASSERT_TRUE(boost::filesystem::exists(m_dbFilePath));
	}

	TEST_F(DbConnectionTest, testLoadDatabaseForHappyPathWithEncryptionOverAnExistingDatabase)
	{
		std::string encryptionKey = "MyEncryptionKey";
		systelab::db::sqlite::Connection connection;
		auto database1 = connection.loadDatabase(systelab::db::sqlite::ConnectionConfiguration(m_dbFilePath.string(), encryptionKey));
		createTable(*database1, "MY_TABLE", 10);
		database1.reset();

		ASSERT_NO_THROW(connection.loadDatabase(systelab::db::sqlite::ConnectionConfiguration(m_dbFilePath.string(), encryptionKey)));
	}


	// Error cases
	TEST_F(DbConnectionTest, testLoadDatabaseForInvalidPathThrowsException)
	{
		boost::filesystem::path invalidFilePath("..ÑD~<>");
		systelab::db::sqlite::Connection connection;
		ASSERT_THROW(connection.loadDatabase(systelab::db::sqlite::ConnectionConfiguration(invalidFilePath.string())), systelab::db::IConnection::Exception);
	}

	TEST_F(DbConnectionTest, testLoadDatabaseForInvalidKeyThrowsException)
	{
		systelab::db::sqlite::Connection connection;
		auto database = connection.loadDatabase(systelab::db::sqlite::ConnectionConfiguration(m_dbFilePath.string(), "MyEncryptionKey"s));
		createTable(*database, "MY_TABLE", 10);
		database.reset();

		ASSERT_THROW(connection.loadDatabase(systelab::db::sqlite::ConnectionConfiguration(m_dbFilePath.string(), "InvalidEncryptionKey"s)), systelab::db::IConnection::Exception);
	}
}}}}
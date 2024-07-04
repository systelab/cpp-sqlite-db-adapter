#include "stdafx.h"
#include "Helpers/Helpers.h"

#include "DbAdapterInterface/IConnection.h"
#include "DbSQLiteAdapter/Connection.h"
#include "DbSQLiteAdapter/ConnectionConfiguration.h"

#include <filesystem>

using namespace testing;

namespace systelab { namespace db { namespace sqlite { namespace unit_test {

	class DbConnectionTest : public testing::Test
	{
	public:
		void SetUp()
		{
			m_dbFilePath = std::filesystem::absolute(std::filesystem::path(std::string("DbConnectionTest") / std::filesystem::path("test.db")));
			std::filesystem::path folderPath = m_dbFilePath.parent_path();
			if (!std::filesystem::exists(folderPath))
			{
				std::filesystem::create_directories(folderPath);
			}

			if (std::filesystem::exists(m_dbFilePath))
			{
				std::filesystem::remove(m_dbFilePath);
			}
		}

		void TearDown()
		{
			if (std::filesystem::exists(m_dbFilePath))
			{
				std::filesystem::remove(m_dbFilePath);
			}
		}

	protected:
		std::filesystem::path m_dbFilePath;
	};


	// Happy path WITHOUT encryption
	TEST_F(DbConnectionTest, testLoadDatabaseForHappyPathWithoutEncryptionReturnsDatabaseObject)
	{
		std::string dbFilepath = m_dbFilePath.string();
		systelab::db::sqlite::ConnectionConfiguration configuration(dbFilepath);

		systelab::db::sqlite::Connection connection;
		auto database = connection.loadDatabase(configuration);
		ASSERT_TRUE(database != NULL);
	}

	TEST_F(DbConnectionTest, testLoadDatabaseForHappyPathWithoutEncryptionCreatesDBFile)
	{
		std::string dbFilepath = m_dbFilePath.string();
		systelab::db::sqlite::ConnectionConfiguration configuration(dbFilepath);

		systelab::db::sqlite::Connection connection;
		auto database = connection.loadDatabase(configuration);
		ASSERT_TRUE(std::filesystem::exists(m_dbFilePath));
	}


	// Happy path WITH encryption
	TEST_F(DbConnectionTest, testLoadDatabaseForHappyPathWithEncryptionReturnsDatabaseObject)
	{
		std::string dbFilepath = m_dbFilePath.string();
		systelab::db::sqlite::ConnectionConfiguration configuration(dbFilepath, "MyEncryptionKey"s);

		systelab::db::sqlite::Connection connection;
		auto database = connection.loadDatabase(configuration);
		ASSERT_TRUE(database != NULL);
	}

	TEST_F(DbConnectionTest, testLoadDatabaseForHappyPathWithEncryptionCreatesDBFile)
	{
		std::string dbFilepath = m_dbFilePath.string();
		systelab::db::sqlite::ConnectionConfiguration configuration(dbFilepath, "MyEncryptionKey"s);

		systelab::db::sqlite::Connection connection;
		auto database = connection.loadDatabase(configuration);
		ASSERT_TRUE(std::filesystem::exists(m_dbFilePath));
	}

	TEST_F(DbConnectionTest, testLoadDatabaseForHappyPathWithEncryptionOverAnExistingDatabase)
	{
		std::string dbFilepath = m_dbFilePath.string();
		std::string encryptionKey = "MyEncryptionKey";
		systelab::db::sqlite::ConnectionConfiguration configuration(dbFilepath, encryptionKey);

		systelab::db::sqlite::Connection connection;
		auto database1 = connection.loadDatabase(configuration);
		createTable(*database1, "MY_TABLE", 10);
		database1.reset();

		ASSERT_NO_THROW(connection.loadDatabase(configuration));
	}


	// Error cases
#ifdef _WIN32
	TEST_F(DbConnectionTest, testLoadDatabaseForInvalidPathThrowsException)
	{
		std::string invalidFilePath = std::filesystem::path("..ÑD~<>").string();
		systelab::db::sqlite::ConnectionConfiguration configuration(invalidFilePath);

		systelab::db::sqlite::Connection connection;
		ASSERT_THROW(connection.loadDatabase(configuration), systelab::db::IConnection::Exception);
	}
#endif

	TEST_F(DbConnectionTest, testLoadDatabaseForInvalidKeyThrowsException)
	{
		std::string dbFilepath = m_dbFilePath.string();
		std::string encryptionKey = "MyEncryptionKey";
		systelab::db::sqlite::ConnectionConfiguration validConfiguration(dbFilepath, encryptionKey);

		systelab::db::sqlite::Connection connection;
		auto database = connection.loadDatabase(validConfiguration);
		createTable(*database, "MY_TABLE", 10);
		database.reset();

		std::string invalidEncryptionKey = "InvalidEncryptionKey";
		systelab::db::sqlite::ConnectionConfiguration invalidConfiguration(dbFilepath, invalidEncryptionKey);
		ASSERT_THROW(connection.loadDatabase(invalidConfiguration), systelab::db::IConnection::Exception);
	}

}}}}

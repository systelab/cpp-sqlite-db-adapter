#include "stdafx.h"
#include "Helpers/Helpers.h"

#include "DbAdapterInterface/IConnection.h"
#include "DbSQLiteAdapter/Connection.h"
#include "DbSQLiteAdapter/ConnectionConfiguration.h"

#include <filesystem>

namespace systelab::db::sqlite::unit_test 
{
	namespace
	{
		using namespace testing;

		static const std::string ENCRYPTION_KEY = "MyEncryptionKey"s;
	}

	class DbConnectionTest : public Test
	{
	public:
		void SetUp() override
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

		void TearDown() override
		{
			if (std::filesystem::exists(m_dbFilePath))
			{
				std::filesystem::remove(m_dbFilePath);
			}
		}

	protected:
		std::filesystem::path m_dbFilePath;
		systelab::db::sqlite::Connection m_connection;

		void createDatabaseContent(systelab::db::sqlite::ConnectionConfiguration configuration)
		{
			systelab::db::sqlite::Connection connection;
			auto database = connection.loadDatabase(configuration);
			createTable(*database, "MY_TABLE"s, 10);
		}
	};


	// Happy path WITHOUT encryption
	TEST_F(DbConnectionTest, testLoadDatabaseForHappyPathWithoutEncryptionReturnsDatabaseObject)
	{
		systelab::db::sqlite::ConnectionConfiguration configuration(m_dbFilePath.string());
		ASSERT_THAT(m_connection.loadDatabase(configuration), NotNull());
	}

	TEST_F(DbConnectionTest, testLoadDatabaseForHappyPathWithoutEncryptionCreatesDBFile)
	{
		systelab::db::sqlite::ConnectionConfiguration configuration(m_dbFilePath.string());
		m_connection.loadDatabase(configuration);
		ASSERT_TRUE(std::filesystem::exists(m_dbFilePath));
	}


	// Happy path WITH encryption
	TEST_F(DbConnectionTest, testLoadDatabaseForHappyPathWithEncryptionReturnsDatabaseObject)
	{
		systelab::db::sqlite::ConnectionConfiguration configuration(m_dbFilePath.string(), ENCRYPTION_KEY);
		ASSERT_THAT(m_connection.loadDatabase(configuration), NotNull());
	}

	TEST_F(DbConnectionTest, testLoadDatabaseForHappyPathWithEncryptionCreatesDBFile)
	{
		systelab::db::sqlite::ConnectionConfiguration configuration(m_dbFilePath.string(), ENCRYPTION_KEY);
		m_connection.loadDatabase(configuration);

		ASSERT_TRUE(std::filesystem::exists(m_dbFilePath));
	}

	TEST_F(DbConnectionTest, testLoadDatabaseForHappyPathWithEncryptionOverAnExistingDatabase)
	{
		systelab::db::sqlite::ConnectionConfiguration configuration(m_dbFilePath.string(), ENCRYPTION_KEY);
		createDatabaseContent(configuration);

		ASSERT_NO_THROW(m_connection.loadDatabase(configuration));
	}


	// Read Only scenarios
	TEST_F(DbConnectionTest, testLoadDatabaseAtReadOnlyMode)
	{
		systelab::db::sqlite::ConnectionConfiguration validConfiguration(m_dbFilePath.string(), ENCRYPTION_KEY);
		createDatabaseContent(validConfiguration);

		systelab::db::sqlite::ConnectionConfiguration readOnlyConfiguration(m_dbFilePath.string(), ENCRYPTION_KEY, true);
		ASSERT_THAT(m_connection.loadDatabase(readOnlyConfiguration), NotNull());
	}

	TEST_F(DbConnectionTest, testLoadDatabaseAtReadOnlyModeCanPerformQueries)
	{
		systelab::db::sqlite::ConnectionConfiguration validConfiguration(m_dbFilePath.string(), ENCRYPTION_KEY);
		createDatabaseContent(validConfiguration);

		systelab::db::sqlite::ConnectionConfiguration readOnlyConfiguration(m_dbFilePath.string(), ENCRYPTION_KEY, true);
		auto database = m_connection.loadDatabase(readOnlyConfiguration);
		auto recordSet = database->executeQuery("SELECT COUNT(1) FROM MY_TABLE");
		ASSERT_EQ(1u, recordSet->getRecordsCount());

		ASSERT_THAT(database->executeQuery("SELECT * FROM not_existent_table"), IsNull());
	}

	TEST_F(DbConnectionTest, testLoadDatabaseAtReadOnlyModeCannotPerformModifications)
	{
		systelab::db::sqlite::ConnectionConfiguration validConfiguration(m_dbFilePath.string(), ENCRYPTION_KEY);
		createDatabaseContent(validConfiguration);

		systelab::db::sqlite::ConnectionConfiguration readOnlyConfiguration(m_dbFilePath.string(), ENCRYPTION_KEY, true);
		auto database = m_connection.loadDatabase(readOnlyConfiguration);

		ASSERT_THROW(database->executeOperation("DROP TABLE MY_TABLE"), std::runtime_error);
	}

	// Error cases
#ifdef _WIN32
	TEST_F(DbConnectionTest, testLoadDatabaseForInvalidPathThrowsException)
	{
		systelab::db::sqlite::ConnectionConfiguration configuration("..ÑD~<>"s);

		ASSERT_THROW(m_connection.loadDatabase(configuration), systelab::db::IConnection::Exception);
	}
#endif

	TEST_F(DbConnectionTest, testLoadDatabaseForInvalidKeyThrowsException)
	{
		systelab::db::sqlite::ConnectionConfiguration validConfiguration(m_dbFilePath.string(), ENCRYPTION_KEY);
		createDatabaseContent(validConfiguration);

		systelab::db::sqlite::ConnectionConfiguration invalidConfiguration(m_dbFilePath.string(), "InvalidEncryptionKey"s);
		ASSERT_THROW(m_connection.loadDatabase(invalidConfiguration), systelab::db::IConnection::Exception);
	}

}

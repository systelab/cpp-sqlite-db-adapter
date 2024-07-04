#include "stdafx.h"
#include "Helpers/Helpers.h"

#include "DbAdapterInterface/IConnection.h"
#include "DbSQLiteAdapter/Connection.h"
#include "DbSQLiteAdapter/ConnectionConfiguration.h"

#include <filesystem>

using namespace testing;

namespace systelab { namespace db { namespace sqlite { namespace unit_test {
		
	static const std::string FOREIGN_KEYS_DATABASE_FILEPATH = "foreign-keys-test.db";
	static const std::string TABLE_1 = "TABLE_1";
	static const std::string TABLE_FOREIGN_KEYS = "TABLE_FOREIGN_KEYS";

	class DbForeignKeysTest: public testing::Test
	{
	public:
		void SetUp()
		{
			if (std::filesystem::exists(FOREIGN_KEYS_DATABASE_FILEPATH))
			{
				std::filesystem::remove(FOREIGN_KEYS_DATABASE_FILEPATH);
			}

			systelab::db::sqlite::ConnectionConfiguration connectionConfiguration(FOREIGN_KEYS_DATABASE_FILEPATH, "keyForTest1234"s);
			m_db = m_connection.loadDatabase(connectionConfiguration);
			
			IDatabase& db = *(m_db.get());
			dropTable(db, TABLE_1);
			dropTable(db, TABLE_FOREIGN_KEYS);
		}

	public:
		std::unique_ptr<IDatabase> m_db;

	protected:
		systelab::db::sqlite::Connection m_connection;
	};


	TEST_F(DbForeignKeysTest, testSQLOperationsDeleteForeignKeyDeleteNoAction)
	{
		IDatabase& db = *(m_db.get());
		createPairOfTables(db, TABLE_1, 100, TABLE_FOREIGN_KEYS, 50, "NO ACTION", "NO ACTION");

		ASSERT_THROW(m_db->executeOperation("DELETE FROM "+TABLE_1+" WHERE ID = 1"), std::exception);

		std::unique_ptr<IRecordSet> recordSet = m_db->executeQuery("SELECT * FROM "+ TABLE_FOREIGN_KEYS +" WHERE FIELD_INT_REF_T1 = 1");

		ASSERT_NE(recordSet->getRecordsCount(), 0);
	}

	TEST_F(DbForeignKeysTest, testSQLOperationsDeleteForeignKeyDeleteCascade)
	{
		IDatabase& db = *(m_db.get());

		createPairOfTables(db, TABLE_1, 100, TABLE_FOREIGN_KEYS, 50,  "CASCADE", "NO ACTION");

		m_db->executeOperation("DELETE FROM "+TABLE_1+" WHERE ID = 1");

		std::unique_ptr<IRecordSet> recordSet = m_db->executeQuery("SELECT * FROM "+ TABLE_FOREIGN_KEYS +" WHERE FIELD_INT_REF_T1 = 1");
		ASSERT_EQ(recordSet->getRecordsCount(), 0);
	}

	TEST_F(DbForeignKeysTest, testSQLOperationsDeleteForeignKeyDeleteRestrict)
	{
		IDatabase& db = *(m_db.get());
		createPairOfTables(db, TABLE_1, 100, TABLE_FOREIGN_KEYS, 50, "RESTRICT", "NO ACTION");

		ASSERT_THROW(m_db->executeOperation("DELETE FROM "+TABLE_1+" WHERE ID = 1"), std::exception);

		std::unique_ptr<IRecordSet> recordSet = m_db->executeQuery("SELECT * FROM "+ TABLE_FOREIGN_KEYS +" WHERE FIELD_INT_REF_T1 = 1");
		ASSERT_NE(recordSet->getRecordsCount(), 0);

	}

	TEST_F(DbForeignKeysTest, testSQLOperationsDeleteForeignKeyDeleteSetNull)
	{
		IDatabase& db = *(m_db.get());
		createPairOfTables(db, TABLE_1, 100, TABLE_FOREIGN_KEYS, 50, "SET NULL", "NO ACTION");

		m_db->executeOperation("DELETE FROM "+TABLE_1+" WHERE ID = 1");

		std::unique_ptr<IRecordSet> recordSet = m_db->executeQuery("SELECT * FROM "+ TABLE_FOREIGN_KEYS +" WHERE FIELD_INT_REF_T1 IS NULL");
		ASSERT_NE(recordSet->getRecordsCount(), 0);
	}

	TEST_F(DbForeignKeysTest, testSQLOperationsUpdateForeignKeyUpdateNoAction)
	{
		IDatabase& db = *(m_db.get());
		createPairOfTables(db, TABLE_1, 100, TABLE_FOREIGN_KEYS, 50, "NO ACTION", "NO ACTION");

		ASSERT_THROW(m_db->executeOperation("UPDATE "+TABLE_1+" SET ID = 1800 WHERE ID = 1"), std::exception);

		std::unique_ptr<IRecordSet> recordSet = m_db->executeQuery("SELECT * FROM "+ TABLE_FOREIGN_KEYS +" WHERE FIELD_INT_REF_T1 = 1");
		ASSERT_NE(recordSet->getRecordsCount(), 0);
	}

	TEST_F(DbForeignKeysTest, testSQLOperationsUpdateForeignKeyUpdateCascade)
	{
		IDatabase& db = *(m_db.get());
		createPairOfTables(db, TABLE_1, 100, TABLE_FOREIGN_KEYS, 50, "NO ACTION", "CASCADE");

		m_db->executeOperation("UPDATE "+TABLE_1+" SET ID = 1800 WHERE ID = 1");

		std::unique_ptr<IRecordSet> recordSet = m_db->executeQuery("SELECT * FROM "+ TABLE_FOREIGN_KEYS +" WHERE FIELD_INT_REF_T1 = 1800");

		ASSERT_NE(recordSet->getRecordsCount(), 0);

		recordSet = m_db->executeQuery("SELECT * FROM "+ TABLE_FOREIGN_KEYS +" WHERE FIELD_INT_REF_T1 = 1");
		ASSERT_EQ(recordSet->getRecordsCount(), 0);
	}

	TEST_F(DbForeignKeysTest, testSQLOperationsUpdateForeignKeyUpdateRestrict)
	{
		IDatabase& db = *(m_db.get());
		createPairOfTables(db, TABLE_1, 100, TABLE_FOREIGN_KEYS, 50, "NO ACTION", "RESTRICT");

		ASSERT_THROW(m_db->executeOperation("UPDATE "+TABLE_1+" SET ID = 1800 WHERE ID = 1"), std::exception);

		std::unique_ptr<IRecordSet> recordSet = m_db->executeQuery("SELECT * FROM "+ TABLE_FOREIGN_KEYS +" WHERE FIELD_INT_REF_T1 = 1");
		ASSERT_NE(recordSet->getRecordsCount(), 0);
	}

	TEST_F(DbForeignKeysTest, testSQLOperationsUpdateForeignKeyUpdateSetNull)
	{
		IDatabase& db = *(m_db.get());		
		createPairOfTables(db, TABLE_1, 100, TABLE_FOREIGN_KEYS, 50, "NO ACTION", "SET NULL");

		m_db->executeOperation("UPDATE "+TABLE_1+" SET ID = 1800 WHERE ID = 1");

		std::unique_ptr<IRecordSet> recordSet = m_db->executeQuery("SELECT * FROM "+ TABLE_FOREIGN_KEYS +" WHERE FIELD_INT_REF_T1 IS NULL");

		ASSERT_NE(recordSet->getRecordsCount(), 0);

		recordSet = m_db->executeQuery("SELECT * FROM "+ TABLE_FOREIGN_KEYS +" WHERE FIELD_INT_REF_T1 = 1");
		ASSERT_EQ(recordSet->getRecordsCount(), 0);
	}

}}}}
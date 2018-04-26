#include "stdafx.h"
#include "Utilities.h"

#include "DbAdapterInterface/IConnection.h"
#include "DbSQLiteAdapter/Connection.h"

#include "TestUtilities/DbAdapter/Mocks/MockConnectionConfiguration.h"


namespace db { namespace unit_test {
		
	using namespace systelab::db;
	using namespace testing;
	using namespace utilities;

	static const std::string TABLE_1 = "TABLE_1";
	static const std::string TABLE_FOREING_KEYS = "TABLE_FOREING_KEYS";

	class DbForeignKeysTest: public testing::Test
	{
	public:
		void SetUp()
		{
			configureConnection(m_connection);
			m_db = m_connection.loadDatabase(m_configuration);
			
			IDatabase& db = *(m_db.get());
			dropTable(db, TABLE_1);
			dropTable(db, TABLE_FOREING_KEYS);	

						
		}

		void TearDown()
		{
			IDatabase& db = *(m_db.get());

			dropTable(db, TABLE_FOREING_KEYS);
			dropTable(db, TABLE_1);		

		}

	public:
		std::unique_ptr<IDatabase> m_db;

	protected:
		systelab::db::sqlite::Connection m_connection;
		systelab::test_utility::MockConnectionConfiguration m_configuration;

		void configureConnection(const systelab::db::sqlite::Connection& connection)
		{
			EXPECT_CALL(m_configuration, getParameter("filepath")).WillRepeatedly(Return("sqlite-test.db"));
		}
		
	};
		
	TEST_F(DbForeignKeysTest, testSQLOperationsDeleteForeignKeyDeleteNoAction)
	{
		IDatabase& db = *(m_db.get());		
		createPairOfTables(db, TABLE_1, 100, TABLE_FOREING_KEYS, 50, "NO ACTION", "NO ACTION");	

		ASSERT_THROW(m_db->executeOperation("DELETE FROM "+TABLE_1+" WHERE ID = 1"), std::exception);

		std::unique_ptr<IRecordSet> recordSet = m_db->executeQuery("SELECT * FROM "+TABLE_FOREING_KEYS+" WHERE FIELD_INT_REF_T1 = 1");

		ASSERT_NE(recordSet->getRecordsCount(), 0);

	}

	TEST_F(DbForeignKeysTest, testSQLOperationsDeleteForeignKeyDeleteCascade)
	{
		IDatabase& db = *(m_db.get());		

		createPairOfTables(db, TABLE_1, 100, TABLE_FOREING_KEYS, 50,  "CASCADE", "NO ACTION");

		m_db->executeOperation("DELETE FROM "+TABLE_1+" WHERE ID = 1");

		std::unique_ptr<IRecordSet> recordSet = m_db->executeQuery("SELECT * FROM "+TABLE_FOREING_KEYS+" WHERE FIELD_INT_REF_T1 = 1");

		ASSERT_EQ(recordSet->getRecordsCount(), 0);
	}

	TEST_F(DbForeignKeysTest, testSQLOperationsDeleteForeignKeyDeleteRestrict)
	{
		IDatabase& db = *(m_db.get());		
		createPairOfTables(db, TABLE_1, 100, TABLE_FOREING_KEYS, 50, "RESTRICT", "NO ACTION");	

		ASSERT_THROW(m_db->executeOperation("DELETE FROM "+TABLE_1+" WHERE ID = 1"), std::exception);

		std::unique_ptr<IRecordSet> recordSet = m_db->executeQuery("SELECT * FROM "+TABLE_FOREING_KEYS+" WHERE FIELD_INT_REF_T1 = 1");

		ASSERT_NE(recordSet->getRecordsCount(), 0);

	}

	TEST_F(DbForeignKeysTest, testSQLOperationsDeleteForeignKeyDeleteSetNull)
	{
		IDatabase& db = *(m_db.get());		
		createPairOfTables(db, TABLE_1, 100, TABLE_FOREING_KEYS, 50, "SET NULL", "NO ACTION");	

		m_db->executeOperation("DELETE FROM "+TABLE_1+" WHERE ID = 1");

		std::unique_ptr<IRecordSet> recordSet = m_db->executeQuery("SELECT * FROM "+TABLE_FOREING_KEYS+" WHERE FIELD_INT_REF_T1 IS NULL");

		ASSERT_NE(recordSet->getRecordsCount(), 0);
	}	

	TEST_F(DbForeignKeysTest, testSQLOperationsUpdateForeignKeyUpdateNoAction)
	{
		IDatabase& db = *(m_db.get());		
		createPairOfTables(db, TABLE_1, 100, TABLE_FOREING_KEYS, 50, "NO ACTION", "NO ACTION");	

		ASSERT_THROW(m_db->executeOperation("UPDATE "+TABLE_1+" SET ID = 1800 WHERE ID = 1"), std::exception);

		std::unique_ptr<IRecordSet> recordSet = m_db->executeQuery("SELECT * FROM "+TABLE_FOREING_KEYS+" WHERE FIELD_INT_REF_T1 = 1");

		ASSERT_NE(recordSet->getRecordsCount(), 0);

	}

	TEST_F(DbForeignKeysTest, testSQLOperationsUpdateForeignKeyUpdateCascade)
	{
		IDatabase& db = *(m_db.get());		
		createPairOfTables(db, TABLE_1, 100, TABLE_FOREING_KEYS, 50, "NO ACTION", "CASCADE");	

		m_db->executeOperation("UPDATE "+TABLE_1+" SET ID = 1800 WHERE ID = 1");

		std::unique_ptr<IRecordSet> recordSet = m_db->executeQuery("SELECT * FROM "+TABLE_FOREING_KEYS+" WHERE FIELD_INT_REF_T1 = 1800");

		ASSERT_NE(recordSet->getRecordsCount(), 0);

		recordSet = m_db->executeQuery("SELECT * FROM "+TABLE_FOREING_KEYS+" WHERE FIELD_INT_REF_T1 = 1");
		ASSERT_EQ(recordSet->getRecordsCount(), 0);

	}

	TEST_F(DbForeignKeysTest, testSQLOperationsUpdateForeignKeyUpdateRestrict)
	{
		IDatabase& db = *(m_db.get());		
		createPairOfTables(db, TABLE_1, 100, TABLE_FOREING_KEYS, 50, "NO ACTION", "RESTRICT");	

		ASSERT_THROW(m_db->executeOperation("UPDATE "+TABLE_1+" SET ID = 1800 WHERE ID = 1"), std::exception);

		std::unique_ptr<IRecordSet> recordSet = m_db->executeQuery("SELECT * FROM "+TABLE_FOREING_KEYS+" WHERE FIELD_INT_REF_T1 = 1");

		ASSERT_NE(recordSet->getRecordsCount(), 0);

	}

	TEST_F(DbForeignKeysTest, testSQLOperationsUpdateForeignKeyUpdateSetNull)
	{
		IDatabase& db = *(m_db.get());		
		createPairOfTables(db, TABLE_1, 100, TABLE_FOREING_KEYS, 50, "NO ACTION", "SET NULL");	

		m_db->executeOperation("UPDATE "+TABLE_1+" SET ID = 1800 WHERE ID = 1");

		std::unique_ptr<IRecordSet> recordSet = m_db->executeQuery("SELECT * FROM "+TABLE_FOREING_KEYS+" WHERE FIELD_INT_REF_T1 IS NULL");

		ASSERT_NE(recordSet->getRecordsCount(), 0);

		recordSet = m_db->executeQuery("SELECT * FROM "+TABLE_FOREING_KEYS+" WHERE FIELD_INT_REF_T1 = 1");
		ASSERT_EQ(recordSet->getRecordsCount(), 0);
	}

}}
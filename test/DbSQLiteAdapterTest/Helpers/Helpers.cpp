#include "stdafx.h"
#include "Utilities.h"

#include "DbAdapterInterface/IConnection.h"
#include "DbSQLiteAdapter/Connection.h"

#include <sstream>


namespace db { namespace unit_test { namespace utilities {
	
	void createTable(IDatabase& db, std::string tableName, unsigned int numRecords)
	{
		db.executeOperation( "CREATE TABLE " + tableName + " " +
							 "(ID INT PRIMARY KEY, " +
							 " FIELD_INT_INDEX INT, "+
							 " FIELD_INT_NO_INDEX INT DEFAULT 2, " +
							 " FIELD_STR_INDEX TEXT, " +
							 " FIELD_STR_NO_INDEX TEXT DEFAULT 'FIELD_STR_NO_INDEX', " +
							 " FIELD_REAL REAL DEFAULT 3.3, " +
							 " FIELD_BOOL BOOLEAN DEFAULT 0, " +
							 " FIELD_DATE DATETIME DEFAULT '20160102T030405') " );

		db.executeOperation( "CREATE INDEX INT_INDEX_" + tableName + " ON " + tableName + "(FIELD_INT_INDEX)" );
		db.executeOperation( "CREATE INDEX STR_INDEX_" + tableName + " ON " + tableName + "(FIELD_STR_INDEX)" );

		std::unique_ptr<ITransaction> transaction = db.startTransaction();

		for (unsigned int i = 0; i < numRecords; i++)
		{
			std::ostringstream oss;
			oss << "INSERT INTO " << tableName << " "
				<< "(ID, FIELD_INT_INDEX, FIELD_INT_NO_INDEX, "
				<< " FIELD_STR_INDEX, FIELD_STR_NO_INDEX, "
				<< " FIELD_REAL, FIELD_BOOL, "
				<< " FIELD_DATE ) "
				<< "VALUES "
				<< "( " << i
				<< ", " << getFieldIntIndexValue(i)
				<< ", " << getFieldIntNoIndexValue(i)
				<< ", " << "'" << getFieldStringIndexValue(i) << "'"
				<< ", " << "'" << getFieldStringNoIndexValue(i) << "'"
				<< ", " << getFieldRealValue(i)
				<< ", " << (getFieldBooleanValue(i) ? 1 : 0)
				<< ", " << "'" << boost::posix_time::to_iso_string(getFieldDateValue(i)) << "'"
				<< " )";

			db.executeOperation(oss.str());
			RowsAffected nRows = db.getRowsAffectedByLastChangeOperation();
			ASSERT_EQ(nRows, 1);
		}

		transaction->commit();
		transaction.reset();
	}

	void createPairOfTables(IDatabase& db,
							std::string tableName_t1, unsigned int numRecords_t1,
							std::string tableName_t2, unsigned int numRecords_t2,
							std::string onDelete, std::string onUpdate)
	{
		db.executeOperation("PRAGMA foreign_keys = ON");
		db.executeOperation("CREATE TABLE "+tableName_t1+"(ID INT PRIMARY KEY NOT NULL, FIELD_INT INT, FIELD_STR CHAR(255))");


		db.executeOperation("CREATE TABLE "+tableName_t2+"(ID INT, FIELD_INT_REF_T1 INT, FIELD_INT INT, FIELD_STR CHAR(255), FOREIGN KEY(FIELD_INT_REF_T1) REFERENCES "+tableName_t1+"(ID) ON DELETE "+onDelete+" ON UPDATE "+onUpdate+")");
		

		std::unique_ptr<ITransaction> transaction = db.startTransaction();
				
		for (unsigned int i = 0; i < numRecords_t1; i++)
		{
			std::ostringstream oss;
		
			oss << "INSERT INTO "+tableName_t1+" (ID, FIELD_INT, FIELD_STR) VALUES (" 
				<< i 
				<< ", " << i%4 
				<< ", \"STR" << i%9 << "\""
				<< ")";
			db.executeOperation(oss.str());
		}

		for (unsigned int i = 0; i < numRecords_t2; i++)
		{
			std::ostringstream oss;
		
			oss << "INSERT INTO "+tableName_t2+" (ID, FIELD_INT_REF_T1, FIELD_INT, FIELD_STR) VALUES (" 
				<< i 
				<< ", " << i%4 
				<< ", " << i%20
				<< ", \"STR" << i%9 << "\""
				<< ")";
			db.executeOperation(oss.str());
		}

		transaction->commit();
		transaction.reset();
	}

	void dropTable(IDatabase& db, const std::string table)
	{
		db.executeOperation("DROP TABLE IF EXISTS "+table);
	}

	int getFieldIntIndexValue(unsigned int id)
	{
		return id % 7;
	}

	int getFieldIntNoIndexValue(unsigned int id)
	{
		return id % 10;
	}

	std::string getFieldStringIndexValue(unsigned int id)
	{
		std::ostringstream oss;
		oss << "STR" << id % 9;
		return oss.str();
	}

	std::string getFieldStringNoIndexValue(unsigned int id)
	{
		std::ostringstream oss; 
		oss << "STR" << id % 12;
		return oss.str();
	}

	double getFieldRealValue(unsigned int id)
	{
		return (id % 13) / 10.;
	}

	bool getFieldBooleanValue(unsigned int id)
	{
		return (id % 2 == 0);
	}

	boost::posix_time::ptime getFieldDateValue(unsigned int id)
	{
		return boost::posix_time::ptime( getFieldDateBaseDate() + boost::gregorian::days(id % 7) );
	}

	boost::posix_time::ptime getFieldDateBaseDate()
	{
		return boost::posix_time::ptime( boost::gregorian::date(2015,1,1) );
	}

	unsigned int getNumRecordsWithFieldIntIndexZero(unsigned int tableRecords)
	{
		return (int) ceil (tableRecords / 7.);
	}

	unsigned int getNumRecordsWithFieldIntNoIndexZero(unsigned int tableRecords)
	{
		return (int) ceil (tableRecords / 10.);
	}

	unsigned int getNumRecordsWithFieldStringIndexZero(unsigned int tableRecords)
	{
		return (int) ceil (tableRecords / 9.);
	}

	unsigned int getNumRecordsWithFieldStringNoIndexZero(unsigned int tableRecords)
	{
		return (int) ceil (tableRecords / 12.);
	}

	unsigned int getNumRecordsWithFieldRealZero(unsigned int tableRecords)
	{
		return (int) ceil (tableRecords / 13.);
	}

	unsigned int getNumRecordsWithFieldBoolTrue(unsigned int tableRecords)
	{
		return (int) ceil (tableRecords / 2.);
	}

	unsigned int getNumRecordsWithFieldDateIsBaseDate(unsigned int tableRecords)
	{
		return (int) ceil (tableRecords / 7.);
	}

}}}

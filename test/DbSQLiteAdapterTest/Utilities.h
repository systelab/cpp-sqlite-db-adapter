#ifndef CPP_SQLITE_DB_ADAPTER_TEST_UTILITIES_H_
#define CPP_SQLITE_DB_ADAPTER_TEST_UTILITIES_H_

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "cpp-db-adapter/IDatabase.h"
#include "cpp-db-adapter/IRecordSet.h"

namespace db {
namespace unit_test {
namespace utilities {

using namespace systelab::db;
using namespace testing;

// DDL operations
void createTable(IDatabase &db, std::string tableName, unsigned int numRecords);
void createPairOfTables(IDatabase &db, std::string tableName_t1,
                        unsigned int numRecords_t1, std::string tableName_t2,
                        unsigned int numRecords_t2, std::string onDelete,
                        std::string onUpdate);

void dropTable(IDatabase &db, const std::string table);

// Field values
int getFieldIntIndexValue(unsigned int id);
int getFieldIntNoIndexValue(unsigned int id);
std::string getFieldStringIndexValue(unsigned int id);
std::string getFieldStringNoIndexValue(unsigned int id);
double getFieldRealValue(unsigned int id);
bool getFieldBooleanValue(unsigned int id);
boost::posix_time::ptime getFieldDateValue(unsigned int id);
boost::posix_time::ptime getFieldDateBaseDate();

// Expected records
unsigned int getNumRecordsWithFieldIntIndexZero(unsigned int tableRecords);
unsigned int getNumRecordsWithFieldIntNoIndexZero(unsigned int tableRecords);
unsigned int getNumRecordsWithFieldStringIndexZero(unsigned int tableRecords);
unsigned int getNumRecordsWithFieldStringNoIndexZero(unsigned int tableRecords);
unsigned int getNumRecordsWithFieldRealZero(unsigned int tableRecords);
unsigned int getNumRecordsWithFieldBoolTrue(unsigned int tableRecords);
unsigned int getNumRecordsWithFieldDateIsBaseDate(unsigned int tableRecords);

} // namespace utilities
} // namespace unit_test
} // namespace db

#endif // CPP_SQLITE_DB_ADAPTER_TEST_UTILITIES_H_

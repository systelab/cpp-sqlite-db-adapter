#ifndef _DBADAPTERINTERFACE_ITABLE_JOSEP_VILA_1712151008_H
#define _DBADAPTERINTERFACE_ITABLE_JOSEP_VILA_1712151008_H

#include "ITableRecordSet.h"
#include "ITableRecord.h"
#include "Types.h"
#include "IFieldValue.h"
#include "IPrimaryKeyValue.h"

#include <string>
#include <memory>
#include <vector>

namespace systelab { namespace db {

	class ITable
	{
	public:
		virtual ~ITable() {}

		virtual std::string getName() const = 0;
		virtual const IPrimaryKey& getPrimaryKey() const = 0;

		virtual unsigned int getFieldsCount() const = 0;
		virtual const IField& getField(unsigned int index) const = 0;
		virtual const IField& getField(const std::string& fieldName) const = 0;

		virtual std::unique_ptr<IFieldValue> createFieldValue(const IField&) const = 0;
		virtual std::unique_ptr<IFieldValue> createFieldValue(const IField&, bool) const = 0;
		virtual std::unique_ptr<IFieldValue> createFieldValue(const IField&, int) const = 0;
		virtual std::unique_ptr<IFieldValue> createFieldValue(const IField&, double) const = 0;
		virtual std::unique_ptr<IFieldValue> createFieldValue(const IField&, const std::string&) const = 0;
		virtual std::unique_ptr<IFieldValue> createFieldValue(const IField&, const boost::posix_time::ptime&) const = 0;
		virtual std::unique_ptr<IFieldValue> createFieldValue(const IField&, std::unique_ptr<IBinaryValue>) const = 0;

		virtual std::unique_ptr<IPrimaryKeyValue> createPrimaryKeyValue() const = 0;

		virtual std::unique_ptr<ITableRecordSet> getAllRecords() const = 0;
		virtual std::unique_ptr<ITableRecord> getRecordByPrimaryKey(const IPrimaryKeyValue&) const = 0;
		virtual std::unique_ptr<ITableRecordSet> filterRecordsByField(const IFieldValue&, const IField* orderByField = NULL) const = 0;
		virtual std::unique_ptr<ITableRecordSet> filterRecordsByFields(const std::vector<IFieldValue*>&, const IField* orderByField = NULL) const = 0;
		virtual std::unique_ptr<ITableRecordSet> filterRecordsByCondition(const std::string& condition) const = 0;
		virtual int getMaxFieldValueInt(const IField&) const = 0;

		virtual std::unique_ptr<ITableRecord> createRecord() const = 0;
		virtual std::unique_ptr<ITableRecord> copyRecord(const ITableRecord&) const = 0;

		virtual RowsAffected insertRecord(ITableRecord&) = 0;
		virtual RowsAffected updateRecord(const ITableRecord&) = 0;
		virtual RowsAffected updateRecord(const std::vector<IFieldValue*>& newValues, const IPrimaryKeyValue&) = 0;
		virtual RowsAffected deleteRecord(const ITableRecord&) = 0;
		virtual RowsAffected deleteRecord(const IPrimaryKeyValue&) = 0;

		virtual RowsAffected updateRecordsByCondition(const std::vector<IFieldValue*>& newValues, const std::vector<IFieldValue*>& conditionValues) = 0;
		virtual RowsAffected deleteRecordsByCondition(const std::vector<IFieldValue*>& conditionValues) = 0;
		virtual RowsAffected deleteRecordsByCondition(const std::string& condition) = 0;

		virtual RowsAffected deleteAllRecords() = 0;
	};

}}

#endif //_DBADAPTERINTERFACE_ITABLE_JOSEP_VILA_1712151008_H
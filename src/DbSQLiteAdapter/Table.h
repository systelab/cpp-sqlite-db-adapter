#pragma once

#include "DbAdapterInterface/ITable.h"

#include "Database.h"

namespace systelab { namespace db { namespace sqlite {

	class Table : public ITable
	{
	public:
		Table(Database& database, const std::string& name);
		~Table();

		std::string getName() const override;
		const IPrimaryKey& getPrimaryKey() const override;

		unsigned int getFieldsCount() const override;
		const IField& getField(unsigned int index) const override;
		const IField& getField(const std::string& fieldName) const override;

		std::unique_ptr<IFieldValue> createFieldValue(const IField&) const override;
		std::unique_ptr<IFieldValue> createFieldValue(const IField&, bool) const override;
		std::unique_ptr<IFieldValue> createFieldValue(const IField&, int) const override;
		std::unique_ptr<IFieldValue> createFieldValue(const IField&, double) const override;
		std::unique_ptr<IFieldValue> createFieldValue(const IField&, const std::string&) const override;
		std::unique_ptr<IFieldValue> createFieldValue(const IField&, const DateTimeType&) const override;
		std::unique_ptr<IFieldValue> createFieldValue(const IField&, std::unique_ptr<IBinaryValue>) const override;

		std::unique_ptr<IPrimaryKeyValue> createPrimaryKeyValue() const override;

		std::unique_ptr<ITableRecordSet> getAllRecords() const override;
		std::unique_ptr<ITableRecord> getRecordByPrimaryKey(const IPrimaryKeyValue&) const override;
		std::unique_ptr<ITableRecordSet> filterRecordsByField(const IFieldValue&, const IField* = NULL) const override;
		std::unique_ptr<ITableRecordSet> filterRecordsByFields(const std::vector<IFieldValue*>&, const IField* = NULL) const override;
		std::unique_ptr<ITableRecordSet> filterRecordsByCondition(const std::string& condition) const override;
		int getMaxFieldValueInt(const IField&) const override;

		std::unique_ptr<ITableRecord> createRecord() const override;
		std::unique_ptr<ITableRecord> copyRecord(const ITableRecord&) const override;

		RowsAffected insertRecord(ITableRecord&) override;
		RowsAffected updateRecord(const ITableRecord&) override;
		RowsAffected updateRecord(const std::vector<IFieldValue*>& newValues, const IPrimaryKeyValue&) override;
		RowsAffected deleteRecord(const ITableRecord&) override;
		RowsAffected deleteRecord(const IPrimaryKeyValue&) override;

		RowsAffected updateRecordsByCondition(const std::vector<IFieldValue*>& newValues, const std::vector<IFieldValue*>& conditionValues) override;
		RowsAffected deleteRecordsByCondition(const std::vector<IFieldValue*>& conditionValues) override;
		RowsAffected deleteRecordsByCondition(const std::string& condition) override;

		RowsAffected deleteAllRecords() override;

	private:
		Database& m_database;
		std::string m_name;
		std::vector< std::unique_ptr<IField> > m_fields;
		std::unique_ptr<IPrimaryKey> m_primaryKey;

		void loadFields();
		FieldTypes getTypeFromSQLiteTypeName(const std::string& typeName);

		bool isOwned(const IField& field) const;
		std::string getSQLValue(const IFieldValue& fieldValue, bool forComparison, bool forAssignment) const;
		std::string getStringList(const std::vector<std::string>& items, const std::string& separator) const;
	};

}}}

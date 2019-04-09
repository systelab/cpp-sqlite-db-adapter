#include "stdafx.h"
#include "Table.h"

#include "Field.h"
#include "FieldValue.h"
#include "TableRecord.h"
#include "PrimaryKey.h"
#include "PrimaryKeyValue.h"

#include "DbAdapterInterface/IDatabase.h"

#include <sqlite3.h>


namespace systelab { namespace db { namespace sqlite {

	Table::Table(Database& database, const std::string& name)
		:m_database(database)
		,m_name(name)
	{
		loadFields();
		m_primaryKey = std::unique_ptr<IPrimaryKey>( new PrimaryKey(*this) );
	}

	Table::~Table()
	{
	}

	std::string Table::getName() const
	{
		return m_name;
	}

	const IPrimaryKey& Table::getPrimaryKey() const
	{
		return *(m_primaryKey.get());
	}

	unsigned int Table::getFieldsCount() const
	{
		return (unsigned int) m_fields.size();
	}

	const IField& Table::getField(unsigned int index) const
	{
		if (index < m_fields.size())
		{
			return *(m_fields[index].get());
		}
		else
		{
			throw std::runtime_error( "Invalid field index" );
		}
	}

	const IField& Table::getField(const std::string& fieldName) const
	{
		unsigned int nFields = (unsigned int) m_fields.size();
		for (unsigned int i = 0; i < nFields; i++)
		{
			if (m_fields[i]->getName() == fieldName)
			{
				return *(m_fields[i].get());
			}
		}

		throw std::runtime_error( "The requested field doesn't exist" );
	}

	std::unique_ptr<IFieldValue> Table::createFieldValue(const IField& field) const
	{
		return std::unique_ptr<IFieldValue>( new FieldValue(field) );
	}

	std::unique_ptr<IFieldValue> Table::createFieldValue(const IField& field, bool value) const
	{
		return std::unique_ptr<IFieldValue>( new FieldValue(field, value) );
	}

	std::unique_ptr<IFieldValue> Table::createFieldValue(const IField& field, int value) const
	{
		return std::unique_ptr<IFieldValue>( new FieldValue(field,value) );
	}

	std::unique_ptr<IFieldValue> Table::createFieldValue(const IField& field, double value) const
	{
		return std::unique_ptr<IFieldValue>( new FieldValue(field,value) );
	}

	std::unique_ptr<IFieldValue> Table::createFieldValue(const IField& field, const std::string& value) const
	{
		return std::unique_ptr<IFieldValue>( new FieldValue(field,value) );
	}

	std::unique_ptr<IFieldValue> Table::createFieldValue(const IField& field, const boost::posix_time::ptime& value) const
	{
		return std::unique_ptr<IFieldValue>( new FieldValue(field,value) );
	}

	std::unique_ptr<IFieldValue> Table::createFieldValue(const IField& field, std::unique_ptr<IBinaryValue> value) const
	{
		return std::unique_ptr<IFieldValue>( new FieldValue(field,std::move(value)) );
	}

	std::unique_ptr<IPrimaryKeyValue> Table::createPrimaryKeyValue() const
	{
		const IPrimaryKey& primaryKey = getPrimaryKey();
		return std::unique_ptr<IPrimaryKeyValue>( new PrimaryKeyValue(primaryKey) );
	}

	std::unique_ptr<ITableRecordSet> Table::getAllRecords() const
	{
		std::string query = "SELECT * FROM " + m_name;
		return m_database.executeTableQuery(query, (ITable&) *this);
	}

	std::unique_ptr<ITableRecord> Table::getRecordByPrimaryKey(const IPrimaryKeyValue& primaryKeyValue) const
	{
		std::vector<IFieldValue*> conditionValues;
		unsigned int nPrimaryKeyFieldValues = primaryKeyValue.getFieldValuesCount();
		for (unsigned int i = 0; i < nPrimaryKeyFieldValues; i++)
		{
			IFieldValue& fieldValue = primaryKeyValue.getFieldValue(i);
			conditionValues.push_back(&fieldValue);
		}

		std::unique_ptr<ITableRecordSet> recordset = filterRecordsByFields(conditionValues);
		if (recordset->getRecordsCount() > 0)
		{
			return recordset->copyCurrentRecord();
		}
		else
		{
			return std::unique_ptr<ITableRecord>();
		}
	}

	std::unique_ptr<ITableRecordSet> Table::filterRecordsByField(const IFieldValue& conditionValue, const IField* orderByField) const
	{
		std::vector<IFieldValue*> conditionValues;
		conditionValues.push_back( &(IFieldValue&) conditionValue );
		return filterRecordsByFields(conditionValues, orderByField);
	}

	std::unique_ptr<ITableRecordSet> Table::filterRecordsByFields(const std::vector<IFieldValue*>& conditionValues, const IField* orderByField) const
	{
		std::vector<std::string> conditionValuesSQL;
		unsigned int nConditionFieldValues = (unsigned int) conditionValues.size();
		for (unsigned int j = 0; j < nConditionFieldValues; j++)
		{
			IFieldValue& conditionFieldValue = *(conditionValues[j]);
			const IField& field = conditionFieldValue.getField();

			if (!isOwned(field))
			{
				throw std::runtime_error("Can't filter by fields that don't come from this table.");
			}

			if (!conditionFieldValue.isDefault())
			{
				std::string conditionFieldValueName = field.getName();
				std::string conditionFieldValueSQLValue = getSQLValue(conditionFieldValue, true, false);
				conditionValuesSQL.push_back( conditionFieldValueName + conditionFieldValueSQLValue );
			}
		}

		std::string conditionSQLStr = getStringList(conditionValuesSQL, " AND ");

		if (orderByField)
		{
			conditionSQLStr += " ORDER BY " + orderByField->getName();
		}

		return filterRecordsByCondition(conditionSQLStr);
	}

	std::unique_ptr<ITableRecordSet> Table::filterRecordsByCondition(const std::string& SQLCondition) const
	{
		std::string query = "SELECT * FROM " + m_name + " WHERE " + SQLCondition;
		return m_database.executeTableQuery(query, (ITable&) *this);
	}

	int Table::getMaxFieldValueInt(const IField& field) const
	{
		std::string query = "SELECT MAX(" + field.getName() + ") FROM " + m_name;
		std::unique_ptr<IRecordSet> result = m_database.executeQuery(query);
		if (result->getRecordsCount() != 1)
		{
			throw "Table::getMaxFieldValueInt returned an invalid amount of results";
		}
		return result->getCurrentRecord().getFieldValue(0).getIntValue();
	}

	std::unique_ptr<ITableRecord> Table::createRecord() const
	{
		std::vector< std::unique_ptr<IFieldValue> > fieldValues;

		unsigned int nFields = (unsigned int) m_fields.size();
		for (unsigned int i = 0; i < nFields; i++)
		{
			IField& field = *m_fields[i].get();
			std::unique_ptr<IFieldValue> fieldValue = createFieldValue(field);
			fieldValue->setDefault();
			fieldValues.push_back( std::move(fieldValue) );
		}

		return std::unique_ptr<ITableRecord>( new TableRecord( (ITable&) *this, fieldValues));
	}

	std::unique_ptr<ITableRecord> Table::copyRecord(const ITableRecord& record) const
	{
		if (&record.getTable() != this)
		{
			throw std::runtime_error("Can't copy records from other tables." );
		}

		std::vector< std::unique_ptr<IFieldValue> > copyFieldValues;
		unsigned int nFieldValues = record.getFieldValuesCount();
		for (unsigned int i = 0; i < nFieldValues; i++)
		{
			IFieldValue& fieldValue = record.getFieldValue(i);
			std::unique_ptr<IFieldValue> copyFieldValue = fieldValue.clone();
			copyFieldValues.push_back( std::move(copyFieldValue) );
		}

		return std::unique_ptr<ITableRecord>( new TableRecord((ITable&) *this, copyFieldValues) );
	}

	RowsAffected Table::insertRecord(ITableRecord& record)
	{
		if (&record.getTable() != this)
		{
			throw std::runtime_error("Can't insert records from other tables." );
		}

		std::vector<std::string> fieldNamesSQL;
		std::vector<std::string> fieldValuesSQL;
		unsigned int nFieldValues = record.getFieldValuesCount();
		for (unsigned int i= 0; i < nFieldValues; i++)
		{
			IFieldValue& fieldValue = record.getFieldValue(i);
			if (!fieldValue.isDefault())
			{
				std::string fieldName = fieldValue.getField().getName();
				fieldNamesSQL.push_back(fieldName);

				std::string fieldValueSQL = getSQLValue(fieldValue, false, false);
				fieldValuesSQL.push_back(fieldValueSQL);
			}
		}

		std::string fieldNamesSQLStr = getStringList(fieldNamesSQL, ",");
		std::string fieldValuesSQLStr = getStringList(fieldValuesSQL, ",");

		std::string insert = "INSERT INTO " + m_name + 
							 " (" + fieldNamesSQLStr + ") " +
							 " VALUES (" + fieldValuesSQLStr + ");";

		Database::Lock databaseLock(m_database);
		m_database.executeOperation(insert);
		RowsAffected rows = m_database.getRowsAffectedByLastChangeOperation();

		if (rows > 0)
		{
			for (unsigned int j = 0; j < nFieldValues; j++)
			{
				IFieldValue& fieldValue = record.getFieldValue(j);
				if (fieldValue.isDefault())
				{
					const IField& field = fieldValue.getField();
					if (field.isPrimaryKey())
					{
						RowId rowId = m_database.getLastInsertedRowId();
						fieldValue.setIntValue(rowId);
					}
					else
					{
						fieldValue.useDefaultValue();
					}
				}
			}
		}

		return rows;
	}

	RowsAffected Table::updateRecord(const ITableRecord& record)
	{
		if (&record.getTable() != this)
		{
			throw std::runtime_error("Can't update records from other tables." );
		}

		std::unique_ptr<IPrimaryKeyValue> primaryKeyValue = createPrimaryKeyValue();
		unsigned int nPrimaryKeyFieldValues = primaryKeyValue->getFieldValuesCount();
		for(unsigned int i = 0; i < nPrimaryKeyFieldValues; i++)
		{
			IFieldValue& primaryKeyFieldValue = primaryKeyValue->getFieldValue(i);
			const IField& primaryKeyField = primaryKeyFieldValue.getField();

			std::string fieldName = primaryKeyField.getName();
			IFieldValue& recordFieldValue = record.getFieldValue(fieldName);
			primaryKeyFieldValue.setValue(recordFieldValue);
		}

		std::vector<IFieldValue*> newValues;
		unsigned int nRecordFieldValues = record.getFieldValuesCount();
		for(unsigned int i = 0; i < nRecordFieldValues; i++)
		{
			IFieldValue& recordFieldValue = record.getFieldValue(i);
			const IField& recordField = recordFieldValue.getField();
			if (!recordField.isPrimaryKey())
			{
				newValues.push_back(&recordFieldValue);
			}
		}

		return updateRecord(newValues, *primaryKeyValue);
	}

	RowsAffected Table::updateRecord(const std::vector<IFieldValue*>& newValues, const IPrimaryKeyValue& primaryKeyValue)
	{
		if (&primaryKeyValue.getTable() != this)
		{
			throw std::runtime_error( "Can't update records using a primary key value from another table." );
		}

		std::vector<IFieldValue*> conditionValues;
		unsigned int nPrimaryKeyFieldValues = primaryKeyValue.getFieldValuesCount();
		for (unsigned int i = 0; i < nPrimaryKeyFieldValues; i++)
		{
			conditionValues.push_back( &primaryKeyValue.getFieldValue(i) );
		}

		return updateRecordsByCondition(newValues, conditionValues);
	}

	RowsAffected Table::deleteRecord(const ITableRecord& record)
	{
		if (&record.getTable() != this)
		{
			throw std::runtime_error("Can't delete records from other tables." );
		}

		std::vector<IFieldValue*> conditionValues;
		unsigned int nFieldValues = record.getFieldValuesCount();
		for(unsigned int i = 0; i < nFieldValues; i++)
		{
			IFieldValue& fieldValue = record.getFieldValue(i);
			if (fieldValue.getField().isPrimaryKey())
			{
				conditionValues.push_back(&fieldValue);
			}
		}

		return deleteRecordsByCondition(conditionValues);
	}

	RowsAffected Table::deleteRecord(const IPrimaryKeyValue& primaryKeyValue)
	{
		if (&primaryKeyValue.getTable() != this)
		{
			throw std::runtime_error( "Can't delete records using a primary key value from another table." );
		}

		std::vector<IFieldValue*> conditionValues;
		unsigned int nPrimaryKeyFieldValues = primaryKeyValue.getFieldValuesCount();
		for (unsigned int i = 0; i < nPrimaryKeyFieldValues; i++)
		{
			conditionValues.push_back( &primaryKeyValue.getFieldValue(i) );
		}

		return deleteRecordsByCondition(conditionValues);
	}

	RowsAffected Table::updateRecordsByCondition(const std::vector<IFieldValue*>& newValues, const std::vector<IFieldValue*>& conditionValues)
	{
		std::vector<std::string> newValuesSQL;
		unsigned int nNewFieldValues = (unsigned int) newValues.size();
		for (unsigned int i = 0; i < nNewFieldValues; i++)
		{
			IFieldValue& newFieldValue = *(newValues[i]);
			const IField& field = newFieldValue.getField();

			if (!isOwned(field))
			{
				throw std::runtime_error("Can't update records using new values that aren't owned by this table." );
			}

			if (!newFieldValue.isDefault())
			{
				std::string newFieldValueName = field.getName();
				std::string newFieldValueSQLValue = getSQLValue(newFieldValue, false, true);
				newValuesSQL.push_back( newFieldValueName + newFieldValueSQLValue );
			}
		}

		std::vector<std::string> conditionValuesSQL;
		unsigned int nConditionFieldValues = (unsigned int) conditionValues.size();
		for (unsigned int j = 0; j < nConditionFieldValues; j++)
		{
			IFieldValue& conditionFieldValue = *(conditionValues[j]);
			const IField& field = conditionFieldValue.getField();

			if (!isOwned(field))
			{
				throw std::runtime_error("Can't update records using condition values that aren't owned by this table." );
			}

			if (!conditionFieldValue.isDefault())
			{
				std::string conditionFieldValueName = field.getName();
				std::string conditionFieldValueSQLValue = getSQLValue(conditionFieldValue, true, false);
				conditionValuesSQL.push_back( conditionFieldValueName + conditionFieldValueSQLValue );
			}
		}

		if (!newValuesSQL.empty() && !conditionValuesSQL.empty())
		{
			std::string newValuesSQLStr = getStringList(newValuesSQL, ", ");
			std::string conditionSQLStr = getStringList(conditionValuesSQL, " AND ");

			std::string update = "UPDATE " + m_name + " " +
								 "SET " + newValuesSQLStr + " " +
								 "WHERE " + conditionSQLStr + ";";

			m_database.executeOperation(update);
			return m_database.getRowsAffectedByLastChangeOperation();
		}
		else
		{
			return (RowsAffected) 0;
		}
	}

	RowsAffected Table::deleteRecordsByCondition(const std::vector<IFieldValue*>& conditionValues)
	{
		std::vector<std::string> conditionValuesSQL;
		unsigned int nConditionFieldValues = (unsigned int) conditionValues.size();
		for (unsigned int i = 0; i < nConditionFieldValues; i++)
		{
			IFieldValue& conditionFieldValue = *(conditionValues[i]);
			const IField& field = conditionFieldValue.getField();

			if (!isOwned(field))
			{
				throw std::runtime_error("Can't update records using condition values that aren't owned by this table." );
			}

			if (!conditionFieldValue.isDefault())
			{
				std::string conditionFieldValueName = field.getName();
				std::string conditionFieldValueSQLValue = getSQLValue(conditionFieldValue, true, false);
				conditionValuesSQL.push_back( conditionFieldValueName + conditionFieldValueSQLValue );
			}
		}

		if (!conditionValuesSQL.empty())
		{
			std::string conditionSQLStr = getStringList(conditionValuesSQL, " AND ");

			std::string deleteSQL = "DELETE FROM " + m_name + " " +
									"WHERE " + conditionSQLStr + ";";

			m_database.executeOperation(deleteSQL);
			return m_database.getRowsAffectedByLastChangeOperation();
		}
		else
		{
			return (RowsAffected) 0;
		}
	}

	RowsAffected Table::deleteRecordsByCondition(const std::string& condition)
	{
		std::string deleteSQL = "DELETE FROM " + m_name + " " +
									"WHERE " + condition + ";";
		m_database.executeOperation(deleteSQL);
		return m_database.getRowsAffectedByLastChangeOperation();
	}

	RowsAffected Table::deleteAllRecords()
	{
		std::string deleteSQL = "DELETE FROM " + m_name + ";";
		m_database.executeOperation(deleteSQL);
		return m_database.getRowsAffectedByLastChangeOperation();
	}

	void Table::loadFields()
	{
		std::string query = "PRAGMA table_info(" + m_name + ")";
		std::unique_ptr<IRecordSet> fieldsRecordSet = m_database.executeQuery(query, true);

		unsigned int i = 0;
		while ( fieldsRecordSet->isCurrentRecordValid() )
		{
			const IRecord& record = fieldsRecordSet->getCurrentRecord();
			std::string fieldName = record.getFieldValue("name").getStringValue();
			std::string fieldTypeName = record.getFieldValue("type").getStringValue();
			FieldTypes fieldType = getTypeFromSQLiteTypeName(fieldTypeName);
			bool fieldPK = (record.getFieldValue("pk").getStringValue() == "1");

			std::string defaultValue = "NULL";
			if (!record.getFieldValue("dflt_value").isNull())
			{
				defaultValue = record.getFieldValue("dflt_value").getStringValue();
				defaultValue = (fieldType == STRING) ? defaultValue.substr(1, defaultValue.size() - 2) : defaultValue;
			}

			std::unique_ptr<IField> fieldCopy( new Field(i, fieldName, fieldType, defaultValue, fieldPK) );
			m_fields.push_back( std::move(fieldCopy) );

			fieldsRecordSet->nextRecord();
			i++;
		}

		if (m_fields.empty())
		{
			std::string excMessage = "Table " + m_name + " doesn't exist in database.";
			throw std::runtime_error(excMessage.c_str());
		}
	}

	FieldTypes Table::getTypeFromSQLiteTypeName(std::string SQLiteTypeName)
	{
		if (SQLiteTypeName == "BOOLEAN")
		{
			return BOOLEAN;
		}
		else if (SQLiteTypeName == "INTEGER" || SQLiteTypeName == "INT")
		{
			return INT;
		}
		else if (SQLiteTypeName == "REAL" || SQLiteTypeName == "DOUBLE")
		{
			return DOUBLE;
		}
		else if (SQLiteTypeName == "TEXT" || SQLiteTypeName.substr(0,4) == "CHAR")
		{
			return STRING;
		}
		else if (SQLiteTypeName == "DATETIME")
		{
			return DATETIME;
		}
		else
		{
			std::string excMessage = "SQLite type name not recognized: " + SQLiteTypeName;
			throw std::runtime_error(excMessage.c_str());
		}
	}

	bool Table::isOwned(const IField& field) const
	{
		unsigned int index = field.getIndex();
		if( index < m_fields.size() )
		{
			return (&(*m_fields[index]) == &field);
		}
		else
		{
			return false;
		}
	}

	std::string Table::getSQLValue(const IFieldValue& fieldValue, bool forComparison, bool forAssignment) const
	{
		std::ostringstream fieldValueStream;
		if (fieldValue.isNull())
		{
			if (forComparison)
			{
				fieldValueStream << " IS ";
			}
			else if (forAssignment)
			{
				fieldValueStream << " = ";
			}

			fieldValueStream << "NULL";
		}
		else
		{
			if (forComparison || forAssignment)
			{
				fieldValueStream << " = ";
			}

			FieldTypes fieldType = fieldValue.getField().getType();
			switch (fieldType)
			{
				case BOOLEAN:
					fieldValueStream << (fieldValue.getBooleanValue() ? 1 : 0);
					break;
				case INT:
					fieldValueStream << fieldValue.getIntValue();
					break;
				case DOUBLE:
					fieldValueStream << std::setprecision(10) << fieldValue.getDoubleValue();
					break;
				case STRING:
					{
						char* value = sqlite3_mprintf("'%q'", fieldValue.getStringValue().c_str());
						fieldValueStream << value ;
						sqlite3_free(value);
					}
					break;
				case DATETIME:
					fieldValueStream << "'" << boost::posix_time::to_iso_string( fieldValue.getDateTimeValue() ) << "'";
					break;
				case BINARY:
					throw std::runtime_error("Insert of tables with binary fields not implemented." );
					break;
				default:
					throw std::runtime_error("Invalid record field type." );
					break;
			}
		}

		return fieldValueStream.str();
	}

	std::string Table::getStringList(const std::vector<std::string>& items, const std::string& separator) const
	{
		std::string to_return = "";

		unsigned int nItems = (unsigned int) items.size();
		for (unsigned int i = 0; i < nItems; i++)
		{
			to_return += (i > 0) ? separator : "";
			to_return += items[i];
		}

		return to_return;
	}

}}}
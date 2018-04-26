#include "stdafx.h"
#include "StubTableRecord.h"

#include "StubFieldValue.h"
#include "StubField.h"


namespace systelab { namespace test_utility {

	StubTableRecord::StubTableRecord(const ITableRecord& other)
	{
		unsigned int nFieldValues = other.getFieldValuesCount();
		for (unsigned int i = 0; i < nFieldValues; i++)
		{
			db::IFieldValue& fieldValue = other.getFieldValue(i);
			m_fieldValues.push_back( std::unique_ptr<StubFieldValue>(new StubFieldValue(fieldValue)));
		}
	}

	StubTableRecord::StubTableRecord(const StubTableRecord& other)
	{
		unsigned int nFieldValues = other.m_fieldValues.size();
		for (unsigned int i = 0; i < nFieldValues; i++)
		{
			m_fieldValues.push_back( std::unique_ptr<StubFieldValue>(new StubFieldValue(*other.m_fieldValues[i].get())) );
		}
	}

	StubTableRecord::StubTableRecord(std::vector< std::unique_ptr<StubFieldValue> >& fieldValues)
	{
		unsigned int nFieldValues = fieldValues.size();
		for( unsigned int i = 0; i < nFieldValues; i++ )
		{
			m_fieldValues.push_back( std::move(fieldValues[i]) );
		}
	}

	StubTableRecord::~StubTableRecord()
	{
	}

	db::ITable& StubTableRecord::getTable() const
	{
		throw std::exception( "Not implemented" );
	}

	unsigned int StubTableRecord::getFieldValuesCount() const
	{
		return m_fieldValues.size();
	}

	StubFieldValue& StubTableRecord::getFieldValue(unsigned int index) const
	{
		if (index < m_fieldValues.size())
		{
			return *(m_fieldValues[index].get());
		}
		else
		{
			throw std::exception( "Invalid field value index" );
		}
	}

	db::IFieldValue& StubTableRecord::getFieldValue(const std::string& fieldName) const
	{
		unsigned int nFields = m_fieldValues.size();
		for (unsigned int i = 0; i < nFields; i++)
		{
			if (m_fieldValues[i]->getField().getName() == fieldName)
			{
				return *(m_fieldValues[i].get());
			}
		}

		throw std::exception( "The requested field value doesn't exist" );
	}

	bool StubTableRecord::hasFieldValue(const std::string& fieldName) const
	{
		unsigned int nFields = m_fieldValues.size();
		for (unsigned int i = 0; i < nFields; i++)
		{
			if (m_fieldValues[i]->getField().getName() == fieldName)
			{
				return true;
			}
		}

		return false;
	}

	std::vector<db::IFieldValue*> StubTableRecord::getValuesList() const
	{
		std::vector<db::IFieldValue*> values;

		unsigned int nRecordFieldValues = getFieldValuesCount();
		for(unsigned int i = 0; i < nRecordFieldValues; i++)
		{
			db::IFieldValue& recordFieldValue = getFieldValue(i);
			const db::IField& recordField = recordFieldValue.getField();
			if (!recordField.isPrimaryKey())
			{
				values.push_back(&recordFieldValue);
			}
		}

		return values;
	}

	StubTableRecord& StubTableRecord::operator= (const StubTableRecord& other)
	{
		unsigned int nFieldValues = other.m_fieldValues.size();
		for (unsigned int i = 0; i < nFieldValues; i++)
		{
			m_fieldValues.push_back( std::unique_ptr<StubFieldValue>(new StubFieldValue(*other.m_fieldValues[i].get())) );
		}

		return *this;
	}

}}

#include "stdafx.h"
#include "PrimaryKey.h"

#include "DbAdapterInterface/ITable.h"

namespace systelab { namespace db { namespace sqlite {

	PrimaryKey::PrimaryKey(ITable& table)
		:m_table(table)
	{
		unsigned int nTableFields = m_table.getFieldsCount();
		for (unsigned int i = 0; i < nTableFields; i++)
		{
			const IField& tableField = m_table.getField(i);
			if (tableField.isPrimaryKey())
			{
				m_fields.push_back(&tableField);
			}
		}
	}

	PrimaryKey::~PrimaryKey()
	{
	}

	ITable& PrimaryKey::getTable() const
	{
		return m_table;
	}

	unsigned int PrimaryKey::getFieldsCount() const
	{
		return (unsigned int) m_fields.size();
	}

	const IField& PrimaryKey::getField(unsigned int index) const
	{
		if (index < m_fields.size())
		{
			return *(m_fields[index]);
		}
		else
		{
			throw std::exception( "Invalid primary key field index" );
		}
	}

	const IField& PrimaryKey::getField(const std::string& fieldName) const
	{
		unsigned int nFields = (unsigned int) m_fields.size();
		for (unsigned int i = 0; i < nFields; i++)
		{
			if (m_fields[i]->getName() == fieldName)
			{
				return *(m_fields[i]);
			}
		}

		throw std::exception( "The requested primary key field doesn't exist" );
	}

}}}
#pragma once

#include "DbAdapterInterface/IPrimaryKey.h"

#include <vector>

namespace systelab { namespace db { namespace sqlite {

	class PrimaryKey : public IPrimaryKey
	{
	public:
		explicit PrimaryKey(ITable& table);
		~PrimaryKey();

		ITable& getTable() const override;

		unsigned int getFieldsCount() const override;
		const IField& getField(unsigned int index) const override;
		const IField& getField(const std::string& fieldName) const override;

	private:
		ITable& m_table;
		std::vector<const IField*> m_fields;

	};

}}}

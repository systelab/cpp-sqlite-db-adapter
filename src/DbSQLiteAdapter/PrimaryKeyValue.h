#pragma once

#include "DbAdapterInterface/IPrimaryKeyValue.h"

#include <memory>
#include <vector>

namespace systelab { namespace db { namespace sqlite {

	class PrimaryKeyValue : public IPrimaryKeyValue
	{
	public:
		explicit PrimaryKeyValue(const IPrimaryKey& primaryKey);
		~PrimaryKeyValue();

		ITable& getTable() const override;
		const IPrimaryKey& getPrimaryKey() const override;

		unsigned int getFieldValuesCount() const override;
		IFieldValue& getFieldValue(unsigned int index) const override;
		IFieldValue& getFieldValue(const std::string& fieldName) const override;

	private:
		const IPrimaryKey& m_primaryKey;
		std::vector< std::unique_ptr<IFieldValue> > m_fieldValues;
	};

}}}

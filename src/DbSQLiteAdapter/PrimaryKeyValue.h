#ifndef _DBSQLITEADAPTER_PRIMARYKEYVALUE_QV_2502161534_H
#define _DBSQLITEADAPTER_PRIMARYKEYVALUE_QV_2502161534_H

#include "DbAdapterInterface/IPrimaryKeyValue.h"

#include <memory>
#include <vector>

namespace systelab { namespace db { namespace sqlite {

	class PrimaryKeyValue : public IPrimaryKeyValue
	{
	public:
		PrimaryKeyValue(const IPrimaryKey& primaryKey);
		virtual ~PrimaryKeyValue();

		ITable& getTable() const;
		const IPrimaryKey& getPrimaryKey() const;

		unsigned int getFieldValuesCount() const;
		IFieldValue& getFieldValue(unsigned int index) const;
		IFieldValue& getFieldValue(const std::string& fieldName) const;

	private:
		const IPrimaryKey& m_primaryKey;
		std::vector< std::unique_ptr<IFieldValue> > m_fieldValues;

	};

}}}

#endif // _DBSQLITEADAPTER_PRIMARYKEYVALUE_QV_2502161534_H
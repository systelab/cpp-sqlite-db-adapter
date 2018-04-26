#ifndef _DBADAPTERTESTUTILITIES_STUBPRIMARYKEYVALUE_DMC_1310161846_H
#define _DBADAPTERTESTUTILITIES_STUBPRIMARYKEYVALUE_DMC_1310161846_H

#include "DbAdapterInterface/IPrimaryKeyValue.h"

#include <memory>
#include <vector>

using namespace systelab::db;

namespace systelab { namespace test_utility { 

	class StubPrimaryKeyValue : public IPrimaryKeyValue
	{
	public:
		StubPrimaryKeyValue(const IPrimaryKey& primaryKey);
		virtual ~StubPrimaryKeyValue();

		ITable& getTable() const;
		const IPrimaryKey& getPrimaryKey() const;

		unsigned int getFieldValuesCount() const;
		IFieldValue& getFieldValue(unsigned int index) const;
		IFieldValue& getFieldValue(const std::string& fieldName) const;

	private:
		const IPrimaryKey& m_primaryKey;
		std::vector< std::unique_ptr<IFieldValue> > m_fieldValues;

	};

}}

#endif // _DBADAPTERTESTUTILITIES_STUBPRIMARYKEYVALUE_DMC_1310161846_H
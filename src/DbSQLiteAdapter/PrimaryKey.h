#ifndef _DBSQLITEADAPTER_PRIMARYKEY_QV_2502161134_H
#define _DBSQLITEADAPTER_PRIMARYKEY_QV_2502161134_H

#include "DbAdapterInterface/IPrimaryKey.h"

#include <vector>

namespace systelab { namespace db { namespace sqlite {

	class PrimaryKey : public IPrimaryKey
	{
	public:
		PrimaryKey(ITable& table);
		virtual ~PrimaryKey();

		ITable& getTable() const;

		unsigned int getFieldsCount() const;
		const IField& getField(unsigned int index) const;
		const IField& getField(const std::string& fieldName) const;

	private:
		ITable& m_table;
		std::vector<const IField*> m_fields;

	};

}}}

#endif // _DBSQLITEADAPTER_PRIMARYKEY_QV_2502161134_H
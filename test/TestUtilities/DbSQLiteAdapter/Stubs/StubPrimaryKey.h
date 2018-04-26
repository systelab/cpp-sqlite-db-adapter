#ifndef _DBADAPTERTESTUTILITIES_STUBPRIMARYKEY_DMC_1310161846_H
#define _DBADAPTERTESTUTILITIES_STUBPRIMARYKEY_DMC_1310161846_H

#include "DbAdapterInterface/IPrimaryKey.h"

using namespace systelab::db;

namespace systelab { namespace test_utility {

	class StubTable;

	class StubPrimaryKey : public db::IPrimaryKey
	{
		public:

			StubPrimaryKey(StubTable& table);
			~StubPrimaryKey(){};

			ITable& getTable() const;

			unsigned int getFieldsCount() const;
			const IField& getField(unsigned int index) const;
			const IField& getField(const std::string& fieldName) const;

			void addField(const IField& tableField);

		private:
			StubTable& m_table;
			std::vector<const IField*> m_fields;
	};

}}

#endif //_DBADAPTERTESTUTILITIES_STUBPRIMARYKEY_DMC_1310161846_H
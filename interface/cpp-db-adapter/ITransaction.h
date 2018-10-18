#ifndef _DBADAPTERINTERFACE_ITRANSACTION_JOSEP_VILA_1712151010_H
#define _DBADAPTERINTERFACE_ITRANSACTION_JOSEP_VILA_1712151010_H

#include "Types.h"

namespace systelab { namespace db {
	class ITransaction
	{
	public:
		virtual ~ITransaction() {}

		virtual void commit() = 0;
		virtual void rollback() = 0;
	};
}}

#endif //_DBADAPTERINTERFACE_ITRANSACTION_JOSEP_VILA_1712151010_H
#ifndef _DBADAPTERINTERFACE_ICONNECTION_JOSEP_VILA_1712150946_H
#define _DBADAPTERINTERFACE_ICONNECTION_JOSEP_VILA_1712150946_H

#include "IDatabase.h"
#include "IConnectionConfiguration.h"

#include <memory>

namespace systelab { namespace db {
	class IConnection
	{
	public:
		virtual ~IConnection() {}

		virtual std::unique_ptr<IDatabase> loadDatabase(IConnectionConfiguration&) = 0;
	};
}}

#endif //_DBADAPTERINTERFACE_ICONNECTION_JOSEP_VILA_1712150946_H
#ifndef _DBADAPTERINTERFACE_ICONNECTIONCONFIGURATION_JOSEP_VILA_1712150946_H
#define _DBADAPTERINTERFACE_ICONNECTIONCONFIGURATION_JOSEP_VILA_1712150946_H

#include <string>

namespace systelab { namespace db {

	class IConnectionConfiguration
	{
	public:
		virtual ~IConnectionConfiguration() {}
		virtual std::string getParameter(const std::string& parameterName) const = 0;
	};
}}

#endif //_DBADAPTERINTERFACE_ICONNECTIONCONFIGURATION_JOSEP_VILA_1712150946_H
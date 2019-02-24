#ifndef _DBSQLITEADAPTER_CONNECTIONCONFIGURATION_QV_2302160917_H
#define _DBSQLITEADAPTER_CONNECTIONCONFIGURATION_QV_2302160917_H

#include "DbAdapterInterface/IConnectionConfiguration.h"

namespace systelab { namespace db { namespace sqlite {

	class ConnectionConfiguration : public IConnectionConfiguration
	{
	public:
		ConnectionConfiguration(const std::string& filepath);
		~ConnectionConfiguration();

		std::string getParameter(const std::string& parameterName) const;

	private:
		std::string m_filepath;
	};

}}}

#endif //_DBSQLITEADAPTER_CONNECTIONCONFIGURATION_QV_2302160917_H
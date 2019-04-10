#include "stdafx.h"
#include "ConnectionConfiguration.h"


namespace systelab { namespace db { namespace sqlite {

	ConnectionConfiguration::ConnectionConfiguration(const std::string& filepath,
													 const boost::optional<std::string>& key)
		:m_filepath(filepath)
		,m_key(key)
	{
	}

	ConnectionConfiguration::~ConnectionConfiguration() = default;

	bool ConnectionConfiguration::hasParameter(const std::string& parameterName) const
	{
		if (parameterName == "filepath")
		{
			return true;
		}
		else if (parameterName == "key")
		{
			return m_key.is_initialized();
		}
		else
		{
			return false;
		}
	}

	std::string ConnectionConfiguration::getParameter(const std::string& parameterName) const
	{
		if (parameterName == "filepath")
		{
			return m_filepath;
		}
		else if (parameterName == "key" && m_key.is_initialized())
		{
			return *m_key;
		}
		else
		{
			return "";
		}
	}

}}}


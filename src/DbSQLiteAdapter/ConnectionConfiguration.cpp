#include "ConnectionConfiguration.h"


namespace systelab { namespace db { namespace sqlite {

	ConnectionConfiguration::ConnectionConfiguration(const std::string& filepath)
		:m_filepath(filepath)
	{
	}

	ConnectionConfiguration::~ConnectionConfiguration()
	{
	}

	std::string ConnectionConfiguration::getParameter(const std::string& parameterName) const
	{
		if (parameterName == "filepath")
		{
			return m_filepath;
		}
		else
		{
			return "";
		}
	}

}}}


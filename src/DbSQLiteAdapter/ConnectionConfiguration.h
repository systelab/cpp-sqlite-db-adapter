#pragma once

#include "DbAdapterInterface/IConnectionConfiguration.h"


namespace systelab { namespace db { namespace sqlite {

	class ConnectionConfiguration : public IConnectionConfiguration
	{
	public:
		ConnectionConfiguration(const std::string& filepath,
								const boost::optional<std::string>& key = boost::none);
		virtual ~ConnectionConfiguration();

		bool hasParameter(const std::string& name) const;
		std::string getParameter(const std::string& name) const;

	private:
		std::string m_filepath;
		boost::optional<std::string> m_key;
	};

}}}


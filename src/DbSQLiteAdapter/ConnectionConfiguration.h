#pragma once

#include "DbAdapterInterface/IConnectionConfiguration.h"

#include <optional>

namespace systelab::db::sqlite 
{
	class ConnectionConfiguration : public IConnectionConfiguration
	{
	public:
		explicit ConnectionConfiguration(const std::string& filepath,
										 const std::optional<std::string>& key = std::nullopt,
										 std::optional<bool> readOnly = std::nullopt);
		~ConnectionConfiguration();

		bool hasParameter(const std::string& name) const override;
		std::string getParameter(const std::string& name) const override;

	private:
		std::string m_filepath;
		std::optional<std::string> m_key;
		std::optional<bool> m_readOnly;
	};
}
#pragma once

#include "DbAdapterInterface/IConnection.h"


namespace systelab { namespace db { namespace sqlite {

	class Connection : public IConnection
	{
	public:
		Connection();
		virtual ~Connection();

		std::unique_ptr<IDatabase> loadDatabase(IConnectionConfiguration&);

	public:
		struct SQLiteException : public Exception
		{
			SQLiteException(const std::string& message,
							const std::string& extendedMessage,
							int errorCode,
							int extendedErrorCode)
				:Exception(message)
				,m_extendedMessage(extendedMessage)
				,m_errorCode(errorCode)
				,m_extendedErrorCode(extendedErrorCode)
			{
			}

			virtual const char* what() const
			{
				std::ostringstream oss;
				oss << std::runtime_error::what() << ": " << m_extendedMessage
					<< "(Error " << m_errorCode << "-" << m_extendedErrorCode << ")" << std::endl;
				return oss.str().c_str();
			}

			std::string m_extendedMessage;
			int m_errorCode;
			int m_extendedErrorCode;
		};
	};

}}}


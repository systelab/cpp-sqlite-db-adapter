#include "stdafx.h"
#include "Connection.h"
#include "Database.h"

#include <DbAdapterInterface/IConnectionConfiguration.h>

#include <sqlite3mc/sqlite3mc.h>

namespace systelab { namespace db { namespace sqlite {

	Connection::Connection()
	{
	}

	Connection::~Connection() = default;

	std::unique_ptr<IDatabase> Connection::loadDatabase(IConnectionConfiguration& configuration)
	{
		sqlite3* database;
		std::string filepath = configuration.getParameter("filepath");
		int openStatusCode = sqlite3_open(filepath.c_str(), &database);
		if (openStatusCode != SQLITE_OK)
		{
			std::string extendedMessage = sqlite3_errmsg(database);
			int extendedErrorCode = sqlite3_extended_errcode(database);
			throw SQLiteException("Unable to open database file '" + filepath + "'", extendedMessage, openStatusCode, extendedErrorCode);
		}

		if (configuration.hasParameter("cipher"))
		{
			auto cipherName = configuration.getParameter("cipher");

			// https://utelle.github.io/SQLite3MultipleCiphers/docs/configuration/config_capi/#description-of-configuration-functions
			// Allowed cypher names: chacha20(default), aes128cbc, aes256cbc, sqlcipher, rc4, ascon128
			int cipherIndex = sqlite3mc_cipher_index(cipherName.c_str());
			if (cipherIndex == -1)
			{
				sqlite3_close(database);
				throw SQLiteException("Invalid cipher '" + cipherName + "'", "", -1, -1);
			}

			// The return value always is the current parameter value on success, or -1 on failure.
			int configCipherStatusCode = sqlite3mc_config(database, "cipher", cipherIndex);
			if (configCipherStatusCode != cipherIndex)
			{
				sqlite3_close(database);
				throw SQLiteException("Unable to configure cipher '" + cipherName + "'", sqlite3_errmsg(database), configCipherStatusCode, sqlite3_extended_errcode(database));
			}
		}

		if (configuration.hasParameter("key"))
		{
			std::string key = configuration.getParameter("key");
			int keyStatusCode = sqlite3_key(database, key.c_str(), (int) key.size());
			if (keyStatusCode != SQLITE_OK)
			{
				std::string extendedMessage = sqlite3_errmsg(database);
				int extendedErrorCode = sqlite3_extended_errcode(database);
				sqlite3_close(database);
				throw SQLiteException("Invalid encryption key", extendedMessage, keyStatusCode, extendedErrorCode);
			}

			auto rc = sqlite3_exec(database, "SELECT 1;", nullptr, nullptr, nullptr);
			if (rc != SQLITE_OK)
			{
				sqlite3_close(database);
				throw SQLiteException("Database access denied", sqlite3_errmsg(database), rc, sqlite3_extended_errcode(database));
			}
		}

		auto db = std::make_unique<Database>(database);
		db->executeOperation("PRAGMA foreign_keys = ON");

		return db;
	}

}}}


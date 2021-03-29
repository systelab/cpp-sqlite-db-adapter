[![Build Status](https://travis-ci.org/systelab/cpp-sqlite-db-adapter.svg?branch=master)](https://travis-ci.org/systelab/cpp-sqlite-db-adapter)
[![Build status](https://ci.appveyor.com/api/projects/status/3qv7apbhgby08ekh?svg=true)](https://ci.appveyor.com/project/systelab/cpp-sqlite-db-adapter)
[![codecov](https://codecov.io/gh/systelab/cpp-sqlite-db-adapter/branch/master/graph/badge.svg)](https://codecov.io/gh/systelab/cpp-sqlite-db-adapter)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/0b13f402745d4421a2f67810a498e49c)](https://www.codacy.com/app/systelab/cpp-sqlite-db-adapter?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=systelab/cpp-sqlite-db-adapter&amp;utm_campaign=Badge_Grade)


# C++ Database Adapter implementation with SQLite

This repository implements the interface for the [C++ Database Adapter](https://github.com/systelab/cpp-db-adapter) using [SQLite](https://www.sqlite.org).


## Setup

### Download using Conan

This library is designed to be installed by making use of [Conan](https://conan.io/) package manager. So, you just need to add the following requirement into your Conan recipe:

```python
def requirements(self):
   self.requires("DbSQLiteAdapter/1.0.0@systelab/stable")
```

> Version number of this code snipped is set just as an example. Replace it for the desired version package to retrieve.

As this package is not available on the conan-center, you will also need to configure a remote repository before installing dependencies:

```bash
conan remote add systelab-public https://csw.jfrog.io/artifactory/api/conan/cpp-conan-production-local
```

See Conan [documentation](https://docs.conan.io/en/latest/) for further details on how to integrate this package with your build system.

### Build from sources

See [BUILD.md](BUILD.md) document for details.


## Usage

Establish connection to a SQLite database by creating a configuration object that specifies the path of `.db` file to open:

```cpp
#include "DbSQLiteAdapter/Connection.h"
#include "DbSQLiteAdapter/ConnectionConfiguration.h"

systelab::db::sqlite::Connection connection;
systelab::db::sqlite::ConnectionConfiguration configuration("MyFolder/MyDatabase.db");
std::unique_ptr<systelab::db::IDatabase> database = connection.loadDatabase(configuration);
```

Use the created `systelab::db::IDatabase` object to access to the database as described on [C++ Database Adapter](https://github.com/systelab/cpp-db-adapter) documentation.

### Encryption

In order work with encryption, the connection configuration object must include a key as the second attribute:

```cpp
systelab::db::sqlite::Connection connection;
systelab::db::sqlite::ConnectionConfiguration configuration("MyFolder/MyDatabase.db", "Y0urDBEncrypt1onK3y");
std::unique_ptr<systelab::db::IDatabase> database = connection.loadDatabase(configuration);
```


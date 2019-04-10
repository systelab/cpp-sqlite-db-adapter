[![Build Status](https://travis-ci.org/systelab/cpp-sqlite-db-adapter.svg?branch=master)](https://travis-ci.org/systelab/cpp-sqlite-db-adapter)
[![Build status](https://ci.appveyor.com/api/projects/status/3qv7apbhgby08ekh?svg=true)](https://ci.appveyor.com/project/systelab/cpp-sqlite-db-adapter)
[![codecov](https://codecov.io/gh/systelab/cpp-sqlite-db-adapter/branch/master/graph/badge.svg)](https://codecov.io/gh/systelab/cpp-sqlite-db-adapter)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/0b13f402745d4421a2f67810a498e49c)](https://www.codacy.com/app/systelab/cpp-sqlite-db-adapter?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=systelab/cpp-sqlite-db-adapter&amp;utm_campaign=Badge_Grade)
[![Download](https://api.bintray.com/packages/systelab/conan/DbSQLiteAdapter:systelab/images/download.svg)](https://bintray.com/systelab/conan/DbSQLiteAdapter:systelab/_latestVersion)

# C++ Database Adapter implementation with SQLite

This repository implements the interface for the [C++ Database Adapter](https://github.com/systelab/cpp-db-adapter) using [SQLite](https://www.sqlite.org).

## Setup

### Build from sources

Prerequisites:
  - [Git](https://git-scm.com/)
  - [Conan](https://conan.io/)
  - [CMake](https://cmake.org/)
  - [Visual Studio](https://visualstudio.microsoft.com/) (only on Windows)
  - [GCC](https://gcc.gnu.org/) (only on Linux)

Build library with the following steps:
  1. Clone this repository in a local drive
  2. Make a build directory (i.e. `build/`)
  3. Install `conan` dependencies in the build directory
  4. Run `cmake` in the build directory to configure build targets
  5. Use `Visual Studio` (on Windows) or `make` (on Linux) to build the library

#### Windows
``` bash
> git clone https://github.com/systelab/cpp-sqlite-db-adapter
> md build && cd build
> conan install .. -s arch=x86
> cmake ..
> devenv.exe DbSQLiteAdapter.sln
```

#### Linux
``` bash
> git clone https://github.com/systelab/cpp-sqlite-db-adapter
> mkdir build && cd build
> conan install ..
> cmake .. -DCMAKE_BUILD_TYPE=[Debug | Coverage | Release]
> make
```

### Download using Conan

  1. Create/update your `conanfile.txt` to add this library as follows:

```
[requires]
DbSQLiteAdapter/1.0.0@systelab/stable

[generators]
cmake
```

  2. Integrate Conan into CMake by adding the following code into your `CMakeLists.txt`:

```cmake
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()
```

  3. Link against `${CONAN_LIBS}` when configuring your executables in CMake:

```cmake
set(MY_PROJECT MyProject)
add_executable(${MY_PROJECT} main.cpp)
target_link_libraries(${MY_PROJECT} ${CONAN_LIBS})
```

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


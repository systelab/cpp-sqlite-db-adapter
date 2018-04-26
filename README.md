# cpp-sqlite-adapter — Adapter for SQLite databases

This C++ library provides an adapter for SQLite databases.

It implements the interface for DB adapters of C++ Systelab projects ([cpp-db-adapter](https://github.com/systelab/cpp-db-adapter))

You can use it to quickly add a persistance to your projects without coupling them to an specific DB engine.

The main features of this library are:
* Built-in basic queries (no need to use SQL)
* Modelization of database schema
* Transactions
* Authentication / encryption (not yet)


## Build the library

To build the library you can simply clone the `cpp-sqlite-db-adapter` repository, install the external libraries and build the Visual Studio solution.

### Clone `cpp-sqlite-db-adapter`

Clone the `cpp-sqlite-db-adapter` repository using git:

```bash
git clone https://github.com/systelab/cpp-sqlite-db-adapter.git
cd cpp-sqlite-db-adapter
```

### Build external libraries

Clone the `vcpkg` repository using git:

```bash
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
```

Then run vcpkg bootstrap batch file:

```bash
.\bootstrap-vcpkg.bat
```

Download and build the following packages (notice that this process may take some minutes):

```bash
.\vcpkg install sqlite3:x64-windows boost-date-time:x64-windows gtest:x64-windows
```

Pack the generated binaries and includes in a folder:

```bash
.\vcpkg export sqlite3:x64-windows boost-date-time:x64-windows gtest:x64-windows --raw
```

Move the packed folder generated on the previous step into the `cpp-sqlite-db-adapter` cloned repository.
Moreover, copy the contents of the subfolder `vcpkg-export-XXXXXXXX-XXXXXX/installed/x64-windows` into `cpp-sqlite-db-adapter/vcpkg` folder.


### Build the Visual Studio solution

TBD

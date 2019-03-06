[![Build Status](https://travis-ci.org/systelab/cpp-sqlite-db-adapter.svg?branch=master)](https://travis-ci.org/systelab/cpp-sqlite-db-adapter)
[![Build status](https://ci.appveyor.com/api/projects/status/3qv7apbhgby08ekh?svg=true)](https://ci.appveyor.com/project/systelab/cpp-sqlite-db-adapter)
[![codecov](https://codecov.io/gh/systelab/cpp-sqlite-db-adapter/branch/master/graph/badge.svg)](https://codecov.io/gh/systelab/cpp-sqlite-db-adapter)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/0b13f402745d4421a2f67810a498e49c)](https://www.codacy.com/app/systelab/cpp-sqlite-db-adapter?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=systelab/cpp-sqlite-db-adapter&amp;utm_campaign=Badge_Grade)

# C++ Database Adapter implementation with SQLite

This repository implements the interface for the [C++ Database Adapter](https://github.com/systelab/cpp-db-adapter) using [SQLite](https://www.sqlite.org).

## Build

Build by making a build directory (i.e. `build/`), install `conan` dependencies and run `cmake` in that dir, and then use `Visual Studio` (on Windows) or `make` (on Linux) to build the desired target.

### Windows

``` bash
> git clone https://github.com/systelab/cpp-sqlite-db-adapter
> md build && cd build
> conan install .. -s arch=x86
> cmake ..
> devenv.exe
```

### Linux

``` bash
> git clone https://github.com/systelab/cpp-sqlite-db-adapter
> mkdir build && cd build
> conan install ..
> cmake .. -DCMAKE_BUILD_TYPE=[Debug | Coverage | Release]
> make
```

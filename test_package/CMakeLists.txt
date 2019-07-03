cmake_minimum_required(VERSION 3.2)

project(DbSQLiteAdapterPackageTestProject)

# Configure environment
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../cmake ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Find tested package
find_package(DbSQLiteAdapter)

# Find required threads package
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

# Configure test package project
set(DB_SQLITE_ADAPTER_PACKAGE_TEST_PROJECT DbSQLiteAdapterPackageTest)
add_executable(${DB_SQLITE_ADAPTER_PACKAGE_TEST_PROJECT} DbSQLiteAdapterExample.cpp)
target_link_libraries(${DB_SQLITE_ADAPTER_PACKAGE_TEST_PROJECT} DbSQLiteAdapter::DbSQLiteAdapter Threads::Threads)

# Register tests
enable_testing()
add_test(NAME DbSQLiteAdapterPackageTest COMMAND ${DB_SQLITE_ADAPTER_PACKAGE_TEST_PROJECT})

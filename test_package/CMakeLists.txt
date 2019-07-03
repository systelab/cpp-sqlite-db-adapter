cmake_minimum_required(VERSION 3.2)

project(DbSQLiteAdapterPackageTestProject)

# Configure environment
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../cmake ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Find tested package and required system libraries
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(DbSQLiteAdapter)

# Configure test package project
set(DB_SQLITE_ADAPTER_PACKAGE_TEST_PROJECT DbSQLiteAdapterPackageTest)
add_executable(${DB_SQLITE_ADAPTER_PACKAGE_TEST_PROJECT} DbSQLiteAdapterExample.cpp)
target_link_libraries(${DB_SQLITE_ADAPTER_PACKAGE_TEST_PROJECT} DbSQLiteAdapter::DbSQLiteAdapter ${CMAKE_THREAD_LIBS_INIT} ${CMAKE_DL_LIBS})

# Register tests
enable_testing()
add_test(NAME DbSQLiteAdapterPackageTest COMMAND ${DB_SQLITE_ADAPTER_PACKAGE_TEST_PROJECT})

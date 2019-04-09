cmake_minimum_required(VERSION 3.2)

project(DbSQLiteAdapterPackageTestProject)

# Configure environment
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Setup 3rd party libraries through conan
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# Configure test package project
set(DB_SQLITE_ADAPTER_PACKAGE_TEST_PROJECT DbSQLiteAdapterPackageTest)
add_executable(${DB_SQLITE_ADAPTER_PACKAGE_TEST_PROJECT} DbSQLiteAdapterExample.cpp)
target_link_libraries(${DB_SQLITE_ADAPTER_PACKAGE_TEST_PROJECT} ${CMAKE_DL_LIBS} ${CONAN_LIBS})

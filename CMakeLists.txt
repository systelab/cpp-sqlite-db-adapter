cmake_minimum_required(VERSION 3.2)

project(DbSQLiteAdapter)

# Configure environment
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Configure include directories
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/sqleet-v0.31.1)
include_directories(${CMAKE_SOURCE_DIR}/test)

# Add subprojects
add_subdirectory(${CMAKE_SOURCE_DIR}/src/sqleet-v0.31.1)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/DbSQLiteAdapter)
add_subdirectory(${CMAKE_SOURCE_DIR}/test/DbSQLiteAdapterTest)

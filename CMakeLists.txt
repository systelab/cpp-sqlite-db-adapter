cmake_minimum_required(VERSION 3.2)

project(DbSQLiteAdapter)

# Configure environment
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Setup up Google test
include(ConfigureGoogleTest)
configure_google_test()

# Setup 3rd party libraries through conan
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()	

# Setup extern repositories
include(GitCloneRepository)
git_clone_repository(URL https://github.com/systelab/cpp-test-utilities.git DIR extern/TestUtilities BRANCH master)
git_clone_repository(URL https://github.com/systelab/cpp-db-adapter.git     DIR extern/DbAdapter     BRANCH master)

# Configure include directories
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/test)

# Add subprojects
add_subdirectory(${CMAKE_SOURCE_DIR}/src/DbSQLiteAdapter)
add_subdirectory(${CMAKE_SOURCE_DIR}/test/DbSQLiteAdapterTest)
add_subdirectory(${CMAKE_SOURCE_DIR}/extern/DbAdapter/DbAdapterTestUtilities)

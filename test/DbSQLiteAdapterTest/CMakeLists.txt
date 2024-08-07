enable_testing()

# Find external dependencides
find_package(DbAdapterTestUtilities)

set(GTEST_PACKAGE_NAME gtest)
find_package(${GTEST_PACKAGE_NAME})
if (NOT TARGET ${GTEST_PACKAGE_NAME}::${GTEST_PACKAGE_NAME})
    set(GTEST_PACKAGE_NAME GTest)
    find_package(${GTEST_PACKAGE_NAME})
endif()

# Configure preprocessor definitions
add_compile_options(-DSQLITE_HAS_CODEC)

# Add project folder into includes
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Configure test project
set(DB_SQLITE_ADAPTER_TEST_PROJECT DbSQLiteAdapterTest)
file(GLOB_RECURSE DB_SQLITE_ADAPTER_TEST_PROJECT_SRC "*.cpp")
file(GLOB_RECURSE DB_SQLITE_ADAPTER_TEST_PROJECT_HDR "*.h")
add_executable(${DB_SQLITE_ADAPTER_TEST_PROJECT} ${DB_SQLITE_ADAPTER_TEST_PROJECT_SRC} ${DB_SQLITE_ADAPTER_TEST_PROJECT_HDR})
target_link_libraries(${DB_SQLITE_ADAPTER_TEST_PROJECT}
					  DbSQLiteAdapter
					  DbAdapterTestUtilities::DbAdapterTestUtilities
					  ${GTEST_PACKAGE_NAME}::${GTEST_PACKAGE_NAME})

#Configure source groups
foreach(FILE ${DB_SQLITE_ADAPTER_TEST_PROJECT_SRC} ${DB_SQLITE_ADAPTER_TEST_PROJECT_HDR}) 
    get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")
    string(REPLACE "/" "\\" GROUP "${GROUP}")

    if ("${FILE}" MATCHES ".*\\.cpp")
       set(GROUP "Source Files${GROUP}")
    elseif("${FILE}" MATCHES ".*\\.h")
       set(GROUP "Header Files${GROUP}")
    endif()

    source_group("${GROUP}" FILES "${FILE}")
endforeach()

# Register tests
add_test(NAME DbSQLiteAdapterTest COMMAND ${DB_SQLITE_ADAPTER_TEST_PROJECT})

# Additional target for code coverage analysis
if (CMAKE_BUILD_TYPE STREQUAL "Coverage")
	include(CodeCoverage)
	setup_target_for_coverage(DbSQLiteAdapterTestCoverage DbSQLiteAdapterTest coverage)
endif()

enable_testing()

include_directories("${gtest_SOURCE_DIR}/include")
include_directories("${gmock_SOURCE_DIR}/include")
message(STATUS "*** gtest Include: ${gtest_SOURCE_DIR}/include")
message(STATUS "*** gmock Include: ${gmock_SOURCE_DIR}/include")

file(GLOB_RECURSE SRC "*.cpp")
file(GLOB_RECURSE HDR "*.h")

add_executable(DbSQLiteAdapterTest ${SRC} ${HDR})
target_link_libraries(DbSQLiteAdapterTest ${Boost_LIBRARIES} cpp-sqlite-db-adapter sqlite3 ${SQLITE3_LINK_FLAGS})

# google_mock
target_link_libraries(DbSQLiteAdapterTest gmock_main)

add_test(NAME DbSQLiteAdapterTest COMMAND DbSQLiteAdapterTest)


if (CMAKE_BUILD_TYPE STREQUAL "Coverage")
  include(CodeCoverage)
  setup_target_for_coverage(${CPP_SQLITE_ADAPTER}_coverage DbSQLiteAdapterTest coverage)

  SET(CMAKE_CXX_FLAGS "-std=c++11 -g -O0 -fprofile-arcs -ftest-coverage")
  SET(CMAKE_C_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
endif() #CMAKE_BUILD_TYPE STREQUAL "Coverage"
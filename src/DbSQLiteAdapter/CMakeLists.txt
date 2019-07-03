cmake_minimum_required(VERSION 3.2)

# Find 3rd party dependencides
find_package(boost)

# Configure preprocessor definitions
add_compile_options(-DBOOST_ALL_DYN_LINK -DSQLITE_HAS_CODEC)

# Add project folder into includes
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Configure RapidJSONAdapter static library
set(DB_SQLITE_ADAPTER DbSQLiteAdapter)
file(GLOB_RECURSE DB_SQLITE_ADAPTER_SRC "*.cpp")
file(GLOB_RECURSE DB_SQLITE_ADAPTER_HDR "*.h")
add_library(${DB_SQLITE_ADAPTER} STATIC ${DB_SQLITE_ADAPTER_SRC} ${DB_SQLITE_ADAPTER_HDR})
target_link_libraries(${DB_SQLITE_ADAPTER} sqleet boost::boost)

#Configure source groups
foreach(FILE ${DB_SQLITE_ADAPTER_SRC} ${DB_SQLITE_ADAPTER_HDR}) 
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

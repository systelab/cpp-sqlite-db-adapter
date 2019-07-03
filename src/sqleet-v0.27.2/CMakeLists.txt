cmake_minimum_required(VERSION 3.2)

# Add project folder into includes
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Find 3rd party libraries
find_package(Threads REQUIRED)

# Configure sqleet static library
set(DB_SQLEET sqleet)
add_library(${DB_SQLEET} STATIC sqlite3.c sqlite3.h)
target_link_libraries(${DB_SQLEET} ${CMAKE_THREAD_LIBS_INIT} ${CMAKE_DL_LIBS} )

# Configure sqlite3.exe
set(DB_SQLITE3 sqlite3)
add_library(${DB_SQLITE3} STATIC shell.c)
target_link_libraries(${DB_SQLITE3} ${DB_SQLEET})


cmake_minimum_required(VERSION 3.2)

macro(include_boost)
    message(STATUS "*** Boost Include: ${Boost_INCLUDE_DIR}")
    message(STATUS "*** Boost Library: ${Boost_LIBRARY_DIRS}")
    message(STATUS "*** Boost Libraries: ${Boost_LIBRARIES}")

    # Include 'boost' directory
    include_directories(${Boost_INCLUDE_DIR})
endmacro()

macro(setup_boost)
    set(BOOST_LIB_DIR "${CMAKE_SOURCE_DIR}/include/boost_1_67_0")
    set(BOOST_BINARY_DIR "${CMAKE_BINARY_DIR}/boost-build")
    set(BOOST_COMPONENTS "date_time")

    set(BOOST_ROOT ${BOOST_BINARY_DIR})
    set(BOOST_INCLUDEDIR "${BOOST_BINARY_DIR}/include")
    set(BOOST_LIBRARYDIR "${BOOST_BINARY_DIR}/lib")

    set(Boost_USE_STATIC_LIBS ON)       # only find static libs
    set(Boost_USE_MULTITHREADED ON)
    set(Boost_USE_STATIC_RUNTIME OFF)
    set(Boost_NO_BOOST_CMAKE ON)
    set(Boost_DEBUG ON)                 # Set to ON to enable debug output from FindBoost.

    find_package(Boost 1.67.0 COMPONENTS ${BOOST_COMPONENTS})
    if(Boost_FOUND)
        include_boost()
    else()
        message(STATUS "\n
                        *******************************************************************************
                        *** DISCLAIMER:  Boost couldn't be found. Don't worry, let's compile it ; ) ***
                        ********************************************************************************\n")

        set(BOOST_COMPRESSED "boost_1_67_0.7z")
        set(ACCM_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
        if (WIN32)
            # Extract Boost
            message(STATUS "*** WINDOWS detected.")
            
            set(7Z_SCRIPT_FILE "${ACCM_INCLUDE_DIR}/Win32/build_7za_Win.bat")
            execute_process(COMMAND ${7Z_SCRIPT_FILE} WORKING_DIRECTORY "${ACCM_INCLUDE_DIR}/Win32" RESULT_VARIABLE result)
            if(result)
                message(FATAL_ERROR "Extracting Boost: ${result}")
            endif()

            if (NOT EXISTS ${BOOST_INCLUDEDIR} OR NOT EXISTS ${BOOST_LIBRARYDIR})
                set(BOOTSTRAP_COMMAND "${BOOST_LIB_DIR}/bootstrap.bat")
                set(B2_COMMAND "${BOOST_LIB_DIR}/b2.exe")
                set(B2_ARGS "install" "--prefix=${BOOST_BINARY_DIR}" "--with-${BOOST_COMPONENTS}")

                # Compile ${BOOST_COMPONENTS}
                # https://www.boost.org/doc/libs/1_67_0/more/getting_started/windows.html#or-build-binaries-from-source
                message(STATUS "COMMAND ${BOOTSTRAP_COMMAND} WORKING_DIRECTORY ${BOOST_LIB_DIR} RESULT_VARIABLE result")
                execute_process(COMMAND ${BOOTSTRAP_COMMAND} WORKING_DIRECTORY ${BOOST_LIB_DIR} RESULT_VARIABLE result)
                if(result)
                    message(FATAL_ERROR "CMake step for boost failed: ${result}")
                endif()
                message(STATUS "COMMAND ${B2_COMMAND} ${B2_ARGS} WORKING_DIRECTORY ${BOOST_LIB_DIR} RESULT_VARIABLE result OUTPUT_QUIET")
                execute_process(COMMAND ${B2_COMMAND} ${B2_ARGS} WORKING_DIRECTORY ${BOOST_LIB_DIR} RESULT_VARIABLE result OUTPUT_QUIET) # OUTPUT_QUIET to avoid all the 'copy' messages
                if(result)
                    message(FATAL_ERROR "CMake step for boost failed: ${result}")
                endif()
            endif()

            # set Boost_* variables manually
            set(Boost_DIR ${BOOST_ROOT})
            set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} ${BOOST_INCLUDEDIR})
            set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${BOOST_LIBRARYDIR})

            set(Boost_INCLUDE_DIR "${BOOST_INCLUDEDIR}/boost-1_67")
            set(Boost_LIBRARY_DIRS ${BOOST_LIBRARYDIR}) # - Default value for Boost_LIBRARY_DIR_RELEASE and Boost_LIBRARY_DIR_DEBUG.
            link_directories(${Boost_LIBRARY_DIRS})

            # set boost libraries manually
            set(BOOST_LIB_TYPE "mt-x64")    # multithread x64
            foreach(lib_boost ${BOOST_COMPONENTS})
                file(GLOB lib_file "${Boost_LIBRARY_DIRS}/libboost_${lib_boost}*${BOOST_LIB_TYPE}*.lib") 
                set(Boost_LIBRARIES ${Boost_LIBRARIES} ${lib_file})
            endforeach(lib_boost)

        elseif(UNIX)
            # extract Boost
            message(STATUS "*** LINUX detected.")

            # compile 7za
            set(7Z_SCRIPT_FILE "${ACCM_INCLUDE_DIR}/POSIX/build_7za.sh")
            message(STATUS "*** COMMAND '${7Z_SCRIPT_FILE} WORKING_DIRECTORY '${ACCM_INCLUDE_DIR}/POSIX' RESULT_VARIABLE result")
            execute_process(COMMAND ${7Z_SCRIPT_FILE} WORKING_DIRECTORY "${ACCM_INCLUDE_DIR}/POSIX" RESULT_VARIABLE result)
            if(result)
              message(FATAL_ERROR "Building 7za: ${result}")
            endif()

            set(7Z_BINARY_FILE "${ACCM_INCLUDE_DIR}/POSIX/p7zip/bin/7za")
            message(STATUS "*** COMMAND bash -c '${7Z_BINARY_FILE} x ${BOOST_COMPRESSED}' WORKING_DIRECTORY ${ACCM_INCLUDE_DIR} RESULT_VARIABLE result")
            execute_process(COMMAND bash -c "${7Z_BINARY_FILE} x ${BOOST_COMPRESSED}" WORKING_DIRECTORY ${ACCM_INCLUDE_DIR} RESULT_VARIABLE result)
            if(result)
              message(FATAL_ERROR "Extracting Boost: ${result}")
            endif()

            # Compile ${BOOST_COMPONENTS}
            # https://www.boost.org/doc/libs/1_67_0/more/getting_started/unix-variants.html#or-build-custom-binaries
            message(STATUS "**** COMMAND bash -c \"${BOOST_LIB_DIR}/bootstrap.sh --prefix=${BOOST_BINARY_DIR} --with-libraries=${BOOST_COMPONENTS}\" WORKING_DIRECTORY ${BOOST_LIB_DIR} RESULT_VARIABLE result OUTPUT_QUIET ****")
            execute_process(COMMAND bash -c "${BOOST_LIB_DIR}/bootstrap.sh --prefix=${BOOST_BINARY_DIR} --with-libraries=${BOOST_COMPONENTS}" WORKING_DIRECTORY ${BOOST_LIB_DIR} RESULT_VARIABLE result OUTPUT_QUIET) # OUTPUT_QUIET to avoid all the 'copy' messages
            if(result)
                message(FATAL_ERROR "CMake step for boost failed: |${result}|")
            endif()
            message(STATUS "COMMAND bash -c \"${BOOST_LIB_DIR}/b2 install\" WORKING_DIRECTORY ${BOOST_LIB_DIR} RESULT_VARIABLE result OUTPUT_QUIET")   # OUTPUT_QUIET to avoid all the 'copy' messages
            execute_process(COMMAND bash -c "${BOOST_LIB_DIR}/b2 install" WORKING_DIRECTORY ${BOOST_LIB_DIR} RESULT_VARIABLE result OUTPUT_QUIET)   # OUTPUT_QUIET to avoid all the 'copy' messages
            # if(result)
            #     message(FATAL_ERROR "Cmake step for boost failed: ${result}")
            # endif()

            # re-find package to set the Boost_* variables
            find_package(Boost 1.67.0 COMPONENTS ${BOOST_COMPONENTS})
            if(NOT Boost_FOUND)
                message(WARNING "Something went wrong, couldn't find boost after compile it.")
            endif()

        else()
            message(FATAL_ERROR "Unsupported platform.")
        endif()

        # include Boost
        include_boost()
    endif()
endmacro()

macro(add_sqlite)
    set(SQLITE3_LIB_DIR "${CMAKE_SOURCE_DIR}/include/sqlite-autoconf-3240000")
    include_directories(${SQLITE3_LIB_DIR})    # -I compilation command
    # https://www.sqlite.org/howtocompile.html
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set(SQLITE3_LINK_FLAGS "-lpthread -ldl")
    endif()
    file(GLOB SQLITE3_SRC "${SQLITE3_LIB_DIR}/sqlite3.c")
endmacro()

macro(setup_sqlite)
    set(SQLITE_LIB_DIR "${CMAKE_SOURCE_DIR}/include/sqlite-autoconf-3240000")
    include_directories(${SQLITE_LIB_DIR})    # -I compilation command
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set(SQLITE3_LINK_FLAGS "-lpthread -ldl")
    endif()
    file(GLOB sqlite_src "${SQLITE_LIB_DIR}/sqlite3.o")
    add_library(sqlite3 STATIC ${SQLITE_LIB_DIR}/sqlite3.c)
#    set(sqlite3 -lthread,-ldl)
#    target_sources(${PROJECT_NAME} "${SQLITE_LIB_DIR}/sqlite3.c")
endmacro()

#macro(setup_my_google_test)
#    message(STATUS "-> Try to compile some google stuff")
#    set(GOOGLE_TEST_LIB_DIR "${CMAKE_SOURCE_DIR}/include/googletest")
#    # set(GOOGLE_TEST_BINARY_DIR "${CMAKE_BINARY_DIR}/google-build")

#    # configure_file(${GOOGLE_TEST_LIB_DIR}/CMakeLists.txt ${GOOGLE_TEST_BINARY_DIR}/CMakeLists.txt COPYONLY)
#    message(status "\n** execute_process(COMMAND ${CMAKE_COMMAND} -G \"${CMAKE_GENERATOR}\" ${GOOGLE_TEST_LIB_DIR} RESULT_VARIABLE result WORKING_DIRECTORY ${CMAKE_BINARY_DIR}")
#    execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" ${GOOGLE_TEST_LIB_DIR} RESULT_VARIABLE result WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
#    if(result)
#        message(FATAL_ERROR "CMake step for googletest failed: ${result}")
#    endif()
#    execute_process(COMMAND ${CMAKE_COMMAND} --build . RESULT_VARIABLE result WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
#    if(result)
#        message(FATAL_ERROR "Build step for googletest failed: ${result}")
#    endif()

#    # Prevent overriding the parent project's compiler/linker
#    # settings on Windows
#    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)


#    include_directories(${GOOGLE_TEST_LIB_DIR}/googletest/include)    # -I compilation command
#    include_directories(${GOOGLE_TEST_LIB_DIR}/googlemock/include)    # -I compilation command
#endmacro()

#macro(setup_google_test)
#    # Download and unpack googletest at configure time
#    configure_file(${CMAKE_SOURCE_DIR}/GoogleTest.in ${CMAKE_BINARY_DIR}/googletest-download/CMakeLists.txt)
#    execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
#                    RESULT_VARIABLE result
#                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download)
#    if(result)
#      message(FATAL_ERROR "CMake step for googletest failed: ${result}")
#    endif()
#    execute_process(COMMAND ${CMAKE_COMMAND} --build .
#      RESULT_VARIABLE result
#      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
#    if(result)
#      message(FATAL_ERROR "Build step for googletest failed: ${result}")
#    endif()

#    # Prevent overriding the parent project's compiler/linker
#    # settings on Windows
#    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

#    # Add googletest directly to our build. This defines
#    # the gtest and gtest_main targets.
#    add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
#                     ${CMAKE_BINARY_DIR}/googletest-build
#                     EXCLUDE_FROM_ALL)
#endmacro()

macro(configure_google_test)
    # see https://github.com/google/googletest/blob/master/googletest/README.md
    # Download and unpack googletest at configurecd time
    configure_file(GoogleTest.in googletest-download/CMakeLists.txt)
    message(STATUS "*** GOOGLE_TEST COMMAND ${CMAKE_COMMAND} -G \"${CMAKE_GENERATOR}\" .
                    RESULT_VARIABLE result
                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download")
    execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
                    RESULT_VARIABLE result
                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download)
    if(result)
      message(FATAL_ERROR "CMake step for googletest failed: ${result}")
    endif()
    execute_process(COMMAND ${CMAKE_COMMAND} --build .
                    RESULT_VARIABLE result
                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download)
    if(result)
      message(FATAL_ERROR "Build step for googletest failed: ${result}")
    endif()

    # Prevent overriding the parent project's compiler/linker
    # settings on Windows
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    # Add googletest directly to our build. This defines
    # the gtest and gtest_main targets.
    add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
                     ${CMAKE_BINARY_DIR}/googletest-build
                     EXCLUDE_FROM_ALL)

    # The gtest/gtest_main targets carry header search path
    # dependencies automatically when using CMake 2.8.11 or
    # later. Otherwise we have to add them here ourselves.
    if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include")
    endif()
endmacro()

# Configure project
project(CppSqlLiteDbAdapter)

# setup google_test (before set bin dir)
configure_google_test()

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_PDB_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# target
set(PROJECT_SQLITE_ADAPTER cpp-sqlite-db-adapter)


# Setup Boost libraries
setup_boost()

# Setup SQLite
setup_sqlite()

# Include interface
include_directories(${CMAKE_SOURCE_DIR}/interface)    # -I compilation command

# Include target
include_directories(${CMAKE_SOURCE_DIR}/src/${PROJECT_SQLITE_ADAPTER})    # -I compilation command

# Add src
add_subdirectory(${CMAKE_SOURCE_DIR}/src)

# Add test
add_subdirectory(${CMAKE_SOURCE_DIR}/test)

# Looking for files
#file(GLOB_RECURSE SRC "src/*.cpp" "src/*.h")

#add_executable(${PROJECT_NAME} "main.cpp" ${SRC} ${SQLITE3_SRC})
#target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} sqlite3 ${SQLITE3_LINK_FLAGS})





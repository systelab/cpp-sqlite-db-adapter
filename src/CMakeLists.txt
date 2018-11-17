# configure cpp-sqlite-db-adapter lib
set(CPP_SQLITE_ADAPTER cpp-sqlite-db-adapter)
file(GLOB CPP_SQLITE_ADAPTER_SRC "${CPP_SQLITE_ADAPTER}/*.cpp")
file(GLOB CPP_SQLITE_ADAPTER_HDR "${CPP_SQLITE_ADAPTER}/*.h")
add_library(${CPP_SQLITE_ADAPTER} STATIC ${CPP_SQLITE_ADAPTER_SRC} ${CPP_SQLITE_ADAPTER_HDR})

# configure demo
set(DEMO demo)
file(GLOB DEMO_SRC "${DEMO}/main.cpp")
add_executable(demo ${DEMO_SRC})
#set_property(TARGET ${DEMO} PROPERTY CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${DEMO}

#message(STATUS "******** OUTPUT: ${CMAKE_BINARY_DIR}")
#set_target_properties(${DEMO}
#    PROPERTIES
#    CMAKE_BINARY_DIR ${CMAKE_BINARY_DIR}/${DEMO}
#)
target_link_libraries(${DEMO} ${CPP_SQLITE_ADAPTER} ${Boost_LIBRARIES})


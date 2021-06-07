cmake_minimum_required(VERSION 3.17.5)
project(scylla_modern_cpp_driver)

set(CMAKE_CXX_STANDARD 17)

set(CASS_BUILD_STATIC ON)
set(CASS_BUILD_SHARED ON)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
add_subdirectory(lib/cpp-driver EXCLUDE_FROM_ALL)

set(SCYLLA_DRIVER_HEADERS
        include/exceptions.hh
        include/prepared_query.hh
        include/query_result.hh
        include/session.hh
        include/statement.hh
        include/value_converters/collection_appender.hh
        include/value_converters/statement_binder.hh
        include/value_converters/tuple_binder.hh
        include/value_converters/value_retriever.hh
        include/scmd.hh
        include/batch_query.hh include/future.hh include/future_callback.hh)

set(SCYLLA_DRIVER_SOURCES
        src/exceptions.cc
        src/prepared_query.cc
        src/query_result.cc
        src/session.cc
        src/statement.cc
        src/batch_query.cc
        src/future.cc
        src/future_callback.cc)

add_library(scylla_modern_cpp_driver SHARED "${SCYLLA_DRIVER_HEADERS}" "${SCYLLA_DRIVER_SOURCES}")
message("CASS INCLUDE: " "${CASS_INCLUDE_DIR}")
# Dirty hack - does anyone know how to do it properly?
target_include_directories(scylla_modern_cpp_driver PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_SOURCE_DIR}/lib/cpp-driver/include")
target_link_libraries(scylla_modern_cpp_driver PUBLIC scylla-cpp-driver)

add_library(scylla_modern_cpp_driver_static STATIC "${SCYLLA_DRIVER_HEADERS}" "${SCYLLA_DRIVER_SOURCES}")
message("CASS INCLUDE: " "${CASS_INCLUDE_DIR}")
# Dirty hack - does anyone know how to do it properly?
target_include_directories(scylla_modern_cpp_driver_static PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_SOURCE_DIR}/lib/cpp-driver/include")
target_link_libraries(scylla_modern_cpp_driver_static PUBLIC cassandra_static)


# We are building as standalone project - build tests
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    add_subdirectory(tests)
endif ()

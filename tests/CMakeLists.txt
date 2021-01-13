find_package(Boost 1.60.0 COMPONENTS system filesystem unit_test_framework REQUIRED )

set(TEST_SRC config.hh fixture.hh basic.cc query_sync.cc query_async.cc query_batch.cc query_prepared.cc main.cc)

add_executable(scmd_tests "${TEST_SRC}")
target_link_libraries(scmd_tests scylla_modern_cpp_driver ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
target_compile_definitions(scmd_tests PUBLIC -DBOOST_TEST_DYN_LINK)
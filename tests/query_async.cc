#include <boost/test/unit_test.hpp>

#include "config.hh"
#include "scmd.hh"
#include "fixture.hh"

#define ARGS(id) (id), 115, 3456, 104755, 123341233124, 30.75, 32342.23, std::string("sadasdasda")
#define TYPES int64_t, int8_t, int16_t, int32_t, int64_t, float, double, std::string
#define ARGS_TUPLE(id) std::make_tuple(ARGS(id))
#define RESULT_TUPLE(r) std::make_tuple(r.get_column<int64_t>("key"), \
                                        r.get_column<int8_t>("a"),    \
                                        r.get_column<int16_t>("b"), \
                                        r.get_column<int32_t>("c"), \
                                        r.get_column<int64_t>("d"), \
                                        r.get_column<float>("e"), \
                                        r.get_column<double>("f"), \
                                        r.get_column<std::string>("g"))



BOOST_FIXTURE_TEST_SUITE(async_query, scylla_fixture)

BOOST_AUTO_TEST_CASE(insert)
{
    scmd::statement stmt("INSERT INTO test_keyspace.test_table (key, a, b, c, d, e, f, g) VALUES ( ?, ?, ?, ?, ?, ?, ?, ?);", 8);
    stmt.bind<TYPES>(ARGS(2 << 16 | 1));
    auto future = session->execute_async(stmt);
    future.wait();
}

BOOST_AUTO_TEST_CASE(select)
{
    scmd::statement stmt("INSERT INTO test_keyspace.test_table (key, a, b, c, d, e, f, g) VALUES ( ?, ?, ?, ?, ?, ?, ?, ?);", 8);
    stmt.bind<TYPES>(ARGS(2 << 16 | 2));
    auto future = session->execute_async(stmt);
    future.wait();

    scmd::statement stmt2("SELECT key, a, b, c, d, e, f, g FROM test_keyspace.test_table WHERE key = ?;", 1);
    stmt2.bind((int64_t)(2 << 16 | 2));
    future = session->execute_async(stmt2);
    scmd::query_result res = future.get_result();


    BOOST_REQUIRE_EQUAL(res.row_count(), 1);
    res.next_row();
    auto row = RESULT_TUPLE(res);
    print_tuple(row);
    BOOST_REQUIRE(ARGS_TUPLE(2 << 16 | 2) == row);
}

BOOST_AUTO_TEST_CASE(too_many_args)
{

    scmd::statement stmt("INSERT INTO test_keyspace.test_table (key, a, b, c, d, e, f, g) VALUES ( ?, ?, ?, ?, ?, ?, ?, ?);", 7);
    stmt.bind<TYPES>(ARGS(2 << 16 | 2));
    auto future = session->execute_async(stmt);
    BOOST_REQUIRE_THROW(future.wait(), scmd::exception);

    scmd::statement stmt2("SELECT key, a, b, c, d, e, f, g FROM test_keyspace.test_table WHERE key = ?;", 0);
    stmt2.bind((int64_t)(2 << 16 | 2));
    BOOST_REQUIRE_THROW(session->execute(stmt2), scmd::exception);
}

BOOST_AUTO_TEST_CASE(not_enough_many_args)
{

    scmd::statement stmt("INSERT INTO test_keyspace.test_table (key, a, b, c, d, e, f, g) VALUES ( ?, ?, ?, ?, ?, ?, ?, ?);", 9);
    stmt.bind<TYPES>(ARGS(2 << 16 | 2));
    auto future = session->execute_async(stmt);
    BOOST_REQUIRE_THROW(future.wait(), scmd::exception);

    scmd::statement stmt2("SELECT key, a, b, c, d, e, f, g FROM test_keyspace.test_table WHERE key = ?;", 2);
    stmt2.bind((int64_t)(2 << 16 | 2));
    BOOST_REQUIRE_THROW(session->execute(stmt2), scmd::exception);
}

BOOST_AUTO_TEST_SUITE_END();
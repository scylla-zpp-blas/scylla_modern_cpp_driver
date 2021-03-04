#include <boost/test/unit_test.hpp>

#include "config.hh"
#include "scmd.hh"
#include "fixture.hh"
#include "utils.hh"


BOOST_FIXTURE_TEST_SUITE(async_query, scylla_fixture)

BOOST_AUTO_TEST_CASE(insert)
{
    scmd::statement stmt("INSERT INTO test_keyspace.test_table (key, a, b, c, d, e, f, g) VALUES ( ?, ?, ?, ?, ?, ?, ?, ?);", 8);
    int64_t id = generate_id();
    stmt.bind<TYPES>(id, ARGS);
    auto future = session->execute_async(stmt);
    future.wait();
}

BOOST_AUTO_TEST_CASE(select)
{
    scmd::statement stmt("INSERT INTO test_keyspace.test_table (key, a, b, c, d, e, f, g) VALUES ( ?, ?, ?, ?, ?, ?, ?, ?);", 8);
    int64_t id = generate_id();
    stmt.bind<TYPES>(id, ARGS);
    auto future = session->execute_async(stmt);
    future.wait();

    scmd::statement stmt2("SELECT key, a, b, c, d, e, f, g FROM test_keyspace.test_table WHERE key = ?;", 1);
    stmt2.bind(id);
    future = session->execute_async(stmt2);
    scmd::query_result res = future.get_result();


    BOOST_REQUIRE_EQUAL(res.row_count(), 1);
    res.next_row();
    auto row = RESULT_TUPLE(res);
    BOOST_REQUIRE(ARGS_TUPLE(id) == row);
}

BOOST_AUTO_TEST_CASE(too_many_args)
{

    scmd::statement stmt("INSERT INTO test_keyspace.test_table (key, a, b, c, d, e, f, g) VALUES ( ?, ?, ?, ?, ?, ?, ?, ?);", 7);
    int64_t id = generate_id();
    BOOST_REQUIRE_THROW(stmt.bind(id, ARGS), scmd::exception);

    scmd::statement stmt2("SELECT key, a, b, c, d, e, f, g FROM test_keyspace.test_table WHERE key = ?;", 0);
    BOOST_REQUIRE_THROW((stmt2.bind(id)), scmd::exception);
}

BOOST_AUTO_TEST_CASE(not_enough_args)
{

    scmd::statement stmt("INSERT INTO test_keyspace.test_table (key, a, b, c, d, e, f, g) VALUES ( ?, ?, ?, ?, ?, ?, ?, ?);", 9);
    int64_t id = generate_id();
    stmt.bind<TYPES>(id, ARGS);
    auto future = session->execute_async(stmt);
    future.wait();

    scmd::statement stmt2("SELECT key, a, b, c, d, e, f, g FROM test_keyspace.test_table WHERE key = ?;", 2);
    stmt2.bind(id);
    BOOST_REQUIRE_THROW(session->execute(stmt2), scmd::exception);;
}

BOOST_AUTO_TEST_SUITE_END();
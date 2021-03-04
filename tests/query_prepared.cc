#include <boost/test/unit_test.hpp>

#include "config.hh"
#include "scmd.hh"
#include "fixture.hh"
#include "utils.hh"


BOOST_FIXTURE_TEST_SUITE(prepared_statement, scylla_fixture)

BOOST_AUTO_TEST_CASE(insert)
{
    scmd::prepared_query prep = session->prepare("INSERT INTO test_keyspace.test_table (key, a, b, c, d, e, f, g) VALUES ( ?, ?, ?, ?, ?, ?, ?, ?);");
    scmd::statement stmt = prep.get_statement();
    int64_t id = generate_id();
    stmt.bind<TYPES>(id, ARGS);
    session->execute(stmt);
}

BOOST_AUTO_TEST_CASE(select)
{
    scmd::prepared_query prep = session->prepare("INSERT INTO test_keyspace.test_table (key, a, b, c, d, e, f, g) VALUES ( ?, ?, ?, ?, ?, ?, ?, ?);");
    scmd::statement stmt = prep.get_statement();
    int64_t id = generate_id();
    stmt.bind<TYPES>(id, ARGS);
    session->execute(stmt);

    prep = session->prepare("SELECT key, a, b, c, d, e, f, g FROM test_keyspace.test_table WHERE key = ?;");
    stmt = prep.get_statement();
    stmt.bind(id);
    scmd::query_result res = session->execute(stmt);

    BOOST_REQUIRE_EQUAL(res.row_count(), 1);
    res.next_row();
    auto row = RESULT_TUPLE(res);
    BOOST_REQUIRE(ARGS_TUPLE(id) == row);
}

BOOST_AUTO_TEST_SUITE_END();
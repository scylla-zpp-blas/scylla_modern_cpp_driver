#include <boost/test/unit_test.hpp>

#include "config.hh"
#include "scmd.hh"
#include "fixture.hh"
#include "utils.hh"


BOOST_FIXTURE_TEST_SUITE(batch_query, scylla_fixture)

BOOST_AUTO_TEST_CASE(insert)
{
    scmd::batch_query batch(CASS_BATCH_TYPE_UNLOGGED);
    for(int i = 0; i < 100; i++) {
        scmd::statement stmt("INSERT INTO test_keyspace.test_table (key, a, b, c, d, e, f, g) VALUES ( ?, ?, ?, ?, ?, ?, ?, ?);", 8);
        int64_t id = generate_id();
        stmt.bind<TYPES>(id, ARGS);
        batch.add_statement(stmt);
    }
    session->execute(batch);
}

BOOST_AUTO_TEST_CASE(select)
{
    scmd::batch_query batch(CASS_BATCH_TYPE_UNLOGGED);
    std::vector<int64_t> ids;
    for(int i = 0; i < 100; i++) {
        scmd::statement stmt("INSERT INTO test_keyspace.test_table (key, a, b, c, d, e, f, g) VALUES ( ?, ?, ?, ?, ?, ?, ?, ?);", 8);
        int64_t id = generate_id();
        ids.push_back(id);
        stmt.bind<TYPES>(id, ARGS);
        batch.add_statement(stmt);
    }
    session->execute(batch);

    for(int i = 0; i < 100; i++) {
        scmd::statement stmt("SELECT key, a, b, c, d, e, f, g FROM test_keyspace.test_table WHERE key = ?;", 1);
        stmt.bind(ids.at(i));
        scmd::query_result res = session->execute(stmt);

        BOOST_REQUIRE_EQUAL(res.row_count(), 1);
        res.next_row();
        auto row = RESULT_TUPLE(res);
        BOOST_REQUIRE(ARGS_TUPLE(ids.at(i)) == row);
    }
}

BOOST_AUTO_TEST_SUITE_END();
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



BOOST_FIXTURE_TEST_SUITE(prepared_statement, scylla_fixture)

BOOST_AUTO_TEST_CASE(insert)
{
    scmd::prepared_query prep = session->prepare("INSERT INTO test_keyspace.test_table (key, a, b, c, d, e, f, g) VALUES ( ?, ?, ?, ?, ?, ?, ?, ?);");
    scmd::statement stmt = prep.get_statement();
    stmt.bind<TYPES>(ARGS(3 << 16 | 1));
    session->execute(stmt);
}

BOOST_AUTO_TEST_CASE(select)
{
    scmd::prepared_query prep = session->prepare("INSERT INTO test_keyspace.test_table (key, a, b, c, d, e, f, g) VALUES ( ?, ?, ?, ?, ?, ?, ?, ?);");
    scmd::statement stmt = prep.get_statement();
    stmt.bind<TYPES>(ARGS(3 << 16 | 2));
    session->execute(stmt);

    prep = session->prepare("SELECT key, a, b, c, d, e, f, g FROM test_keyspace.test_table WHERE key = ?;");
    stmt = prep.get_statement();
    stmt.bind((int64_t)(3 << 16 | 2));
    scmd::query_result res = session->execute(stmt);

    BOOST_REQUIRE_EQUAL(res.row_count(), 1);
    res.next_row();
    auto row = RESULT_TUPLE(res);
    print_tuple(row);
    BOOST_REQUIRE(ARGS_TUPLE(3 << 16 | 2) == row);
}

BOOST_AUTO_TEST_SUITE_END();
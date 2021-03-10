#include <boost/test/unit_test.hpp>

#include "config.hh"
#include "scmd.hh"

namespace utf = boost::unit_test;

BOOST_AUTO_TEST_SUITE(Basic)

BOOST_AUTO_TEST_CASE(connect)
{
    global_config::init();
    std::unique_ptr<scmd::session> session = nullptr;
    session = std::make_unique<scmd::session>(global_config::scylla_ip, global_config::scylla_port);
}

BOOST_AUTO_TEST_CASE(connect_wrong)
{
    global_config::init();
    std::unique_ptr<scmd::session> session = nullptr;
    BOOST_REQUIRE_THROW({session = std::make_unique<scmd::session>("0.0.0.0", global_config::scylla_port); }, scmd::exception);
}

BOOST_AUTO_TEST_CASE(drop_keyspace)
{
    global_config::init();
    std::unique_ptr<scmd::session> session = nullptr;
    session = std::make_unique<scmd::session>(global_config::scylla_ip, global_config::scylla_port);
    session->execute(R"(
            CREATE KEYSPACE IF NOT EXISTS test_keyspace WITH REPLICATION = {
                'class' : 'SimpleStrategy',
                'replication_factor' : 1
            };)");

    session->execute(R"(DROP KEYSPACE test_keyspace;)");
}

BOOST_AUTO_TEST_CASE(create_keyspace)
{
    global_config::init();
    std::unique_ptr<scmd::session> session = nullptr;
    session = std::make_unique<scmd::session>(global_config::scylla_ip, global_config::scylla_port);
    session->execute(R"(
        CREATE KEYSPACE test_keyspace WITH REPLICATION = {
            'class' : 'SimpleStrategy',
            'replication_factor' : 1
        };)");
}

BOOST_AUTO_TEST_CASE(move_semantics)
{
    global_config::init();
    std::vector<scmd::session> sess_v;
    sess_v.emplace_back(global_config::scylla_ip, global_config::scylla_port);
    BOOST_REQUIRE_EQUAL(sess_v.size(), 1);
    scmd::session session = std::move(sess_v.back());
    // Alternative version:
    // scmd::session session(std::move(sess_v.back()));
    session.execute(R"(
            CREATE KEYSPACE IF NOT EXISTS test_keyspace WITH REPLICATION = {
                'class' : 'SimpleStrategy',
                'replication_factor' : 1
            };)");

    session.execute(R"(DROP KEYSPACE test_keyspace;)");
}

BOOST_AUTO_TEST_SUITE_END();
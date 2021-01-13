#pragma once

#include "scmd.hh"
#include "config.hh"

class scylla_fixture {
public:
    std::unique_ptr<scmd::session> session;

    scylla_fixture() : session(nullptr) {
        global_config::init();
        connect();
        drop_keyspace();
        create_keyspace();
        create_table();
    }

    ~scylla_fixture() {
        drop_keyspace();
    }

    void connect(std::string ip = global_config::scylla_ip, std::string port = global_config::scylla_port) {
        this->session = std::make_unique<scmd::session>(ip, port);
    }

    void create_keyspace() {
        session->execute(R"(
            CREATE KEYSPACE IF NOT EXISTS test_keyspace WITH REPLICATION = {
                'class' : 'SimpleStrategy',
                'replication_factor' : 1
            };
        )");
    }

    void drop_keyspace() {
        session->execute(R"(
            DROP KEYSPACE IF EXISTS test_keyspace;
        )");
    }

    void create_table() {
        session->execute(R"(
            CREATE TABLE test_keyspace.test_table (
                key bigint,
                a tinyint,
                b smallint,
                c int,
                d bigint,
                e float,
                f double,
                g varchar,
                PRIMARY KEY (key)
            );
        )");
    }
};

template<class TupType, size_t... I>
void print_tuple (const TupType& _tup, std::index_sequence<I...>)
{
    std::cout << "(";
    (..., (std::cout << (I == 0? "" : ", ") << std::get<I>(_tup)));
    std::cout << ")\n";
}

template<class... T>
void print_tuple (const std::tuple<T...>& _tup)
{
    print_tuple(_tup, std::make_index_sequence<sizeof...(T)>());
}

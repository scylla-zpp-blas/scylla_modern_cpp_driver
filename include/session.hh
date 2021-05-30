#pragma once

#include <string>

#include "batch_query.hh"
#include "cassandra.h"
#include "future.hh"
#include "prepared_query.hh"
#include "query_result.hh"
#include "statement.hh"

namespace scmd {
// TODO: divide into cluster and session, do this class better
class session {
    CassCluster *_cluster;
    CassSession *_session;

public:
    explicit session(const std::string &address, const std::string &port = "");

    // We can't really copy this class
    session(const session &other) = delete;
    session &operator=(const session &other) = delete;

    session(session &&other) noexcept;
    session &operator=(session &&other) noexcept;

    ~session();

    prepared_query prepare(const std::string &query);

    const CassPrepared *prepare_raw(const std::string& query);

    future execute_async(const statement &statement);
    future execute_async(statement &&statement);

    // Convenience functions to avoid creating scd_statement manually for simple queries
    future execute_async(const std::string &query);
    future execute_async(std::string &&query);

    future execute_async(const batch_query &query);
    future execute_async(batch_query &&query);

    future execute_async(const prepared_query &query);
    future execute_async(prepared_query &&query);

    template<typename... Args,
        typename = typename std::enable_if<0 != sizeof...(Args)>::type>
    future execute_async(const std::string &query, Args... args) {
        return execute_async(scmd::statement(query, sizeof...(args)).bind(args...));
    }
    template<typename... Args,
        typename = typename std::enable_if<0 != sizeof...(Args)>::type>
    future execute_async(std::string &&query, Args... args) {
        return execute_async(scmd::statement(std::move(query), sizeof...(args)).bind(args...));
    }

    template<typename... Args,
        typename = typename std::enable_if<0 != sizeof...(Args)>::type>
    future execute_async(statement &statement, Args... args) {
        return execute_async(statement.bind(args...));
    }
    template<typename... Args,
        typename = typename std::enable_if<0 != sizeof...(Args)>::type>
    future execute_async(statement &&statement, Args... args) {
        return execute_async(statement.bind(args...));
    }

    template<typename... Args,
        typename = typename std::enable_if<0 != sizeof...(Args)>::type>
    future execute_async(const prepared_query &query, Args... args) {
        return execute_async(query.get_statement().bind(args...));
    }
    template<typename... Args,
        typename = typename std::enable_if<0 != sizeof...(Args)>::type>
    future execute_async(prepared_query &&query, Args... args) {
        return execute_async(query.get_statement().bind(args...));
    }


    query_result execute(const statement &statement);
    query_result execute(statement &&statement);

    // Convenience functions to avoid creating scd_statement manually for simple queries
    query_result execute(const std::string &query);
    query_result execute(std::string &&query);

    query_result execute(const batch_query &query);
    query_result execute(batch_query &&query);

    query_result execute(const prepared_query &query);
    query_result execute(prepared_query &&query);

    template<typename... Args,
            typename = typename std::enable_if<0 != sizeof...(Args)>::type>
    query_result execute(const std::string &query, Args... args) {
        return execute_async(scmd::statement(query, sizeof...(args)).bind(args...)).get_result();
    }
    template<typename... Args,
        typename = typename std::enable_if<0 != sizeof...(Args)>::type>
    query_result execute(std::string &&query, Args... args) {
        return execute_async(scmd::statement(std::move(query), sizeof...(args)).bind(args...)).get_result();
    }

    template<typename... Args,
        typename = typename std::enable_if<0 != sizeof...(Args)>::type>
    query_result execute(statement &statement, Args... args) {
        return execute_async(statement.bind(args...)).get_result();
    }
    template<typename... Args,
        typename = typename std::enable_if<0 != sizeof...(Args)>::type>
    query_result execute(statement &&statement, Args... args) {
        return execute_async(statement.bind(args...)).get_result();
    }

    template<typename... Args,
        typename = typename std::enable_if<0 != sizeof...(Args)>::type>
    query_result execute(const prepared_query& query, Args... args) {
        return execute_async(query.get_statement().bind(args...)).get_result();
    }
    template<typename... Args,
        typename = typename std::enable_if<0 != sizeof...(Args)>::type>
    query_result execute(prepared_query &&query, Args... args) {
        return execute_async(query.get_statement().bind(args...)).get_result();
    }
};
}  // namespace scmd

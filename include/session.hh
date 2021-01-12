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

    future execute_async(const statement &statement);

    // Convenience functions to avoid creating scd_statement manually for simple queries
    future execute_async(const std::string &query);

    future execute_async(const batch_query& query);

    template<typename... Args,
        typename = typename std::enable_if<0 != sizeof...(Args)>::type>
    future execute_async(const std::string &query, Args... args) {
        return execute_async(scmd::statement(query).bind(args...));
    }

    query_result execute(const statement &statement);

    // Convenience functions to avoid creating scd_statement manually for simple queries
    query_result execute(const std::string &query);

    query_result execute(const batch_query& query);

    template<typename... Args,
            typename = typename std::enable_if<0 != sizeof...(Args)>::type>
    query_result execute(const std::string &query, Args... args) {
        return execute_async(scmd::statement(query).bind(args...)).get_result();
    }



    prepared_query prepare(std::string query);
};
}  // namespace scmd

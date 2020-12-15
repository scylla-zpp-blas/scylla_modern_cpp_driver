#pragma once

#include <string>

#include "cassandra.h"
#include "query_result.hh"
#include "statement.hh"
#include "prepared_query.hh"

namespace scmd {
    //TODO: divide into cluster and session, do this class better
    class session {
        CassCluster* _cluster;
        CassSession* _session;

    public:
        explicit session(const std::string& address, const std::string& port = "");

        // We can't really copy this class
        session(const session& other) = delete;
        session& operator=(const session& other) = delete;

        session(session&& other) noexcept;
        session& operator=(session&& other) noexcept;

        query_result execute(const statement& statement);
        // Convenience function to avoid creating scd_statement manually for simple queries
        query_result execute(const std::string& query);

        prepared_query prepare(std::string query);

        ~session();
    };
}


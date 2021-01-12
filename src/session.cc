#include "cassandra.h"

#include "session.hh"
#include "future.hh"
#include "exceptions.hh"

namespace scmd {

session::session(const std::string &address, const std::string &port) {
    _cluster = cass_cluster_new();
    _session = cass_session_new();

    cass_cluster_set_contact_points(_cluster, address.c_str());

    if (!port.empty()) {
        cass_cluster_set_port(_cluster, std::stoi(port));
    }

    cass_cluster_set_protocol_version(_cluster, CASS_PROTOCOL_VERSION_V4);
    CassFuture *connect_future = cass_session_connect(_session, _cluster);
    auto cleanup = [=]() { cass_future_free(connect_future); };

    scmd_internal::throw_on_cass_error(cass_future_error_code(connect_future), cleanup);
    cleanup();
}

session::session(session &&other) noexcept
    : _cluster(std::exchange(other._cluster, nullptr)), _session(std::exchange(other._session, nullptr)) {}

session &session::operator=(session &&other) noexcept {
    std::swap(_cluster, other._cluster);
    std::swap(_session, other._session);
    return *this;
}

session::~session() {
    cass_cluster_free(_cluster);
    cass_session_free(_session);
}

future session::execute_async(const statement &statement) {
    return future(cass_session_execute(_session, statement.get_statement()));
}

future session::execute_async(const std::string &query) {
    return execute_async(statement(query));
}

future session::execute_async(const batch_query &query) {
    return future(cass_session_execute_batch(_session, query.get_query()));
}

query_result session::execute(const statement &statement) {
    return execute_async(statement).get_result();
}

query_result session::execute(const std::string &query) {
    return execute_async(query).get_result();
}

query_result session::execute(const batch_query &query) {
    return execute_async(query).get_result();
}

prepared_query session::prepare(std::string query) {
    scmd::future future(cass_session_prepare(_session, query.c_str()));
    return future.get_prepared();
}

}  // namespace scmd

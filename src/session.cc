#include "cassandra.h"

#include "session.hh"
#include "future.hh"
#include "exceptions.hh"

namespace scmd {

session::session(const std::string &address, const std::string &port) {
    _cluster = cass_cluster_new();
    _session = cass_session_new();

    cass_cluster_set_contact_points(_cluster, address.c_str());
    //cass_cluster_set_local_port_range(_cluster, 50000, 60000);

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
    if(this->_session) {
        cass_session_free(_session);
    }
    if(this->_cluster) {
        cass_cluster_free(_cluster);
    }
}

prepared_query session::prepare(const std::string& query) {
    scmd::future future(cass_session_prepare(_session, query.c_str()));
    return future.get_prepared();
}

const CassPrepared *session::prepare_raw(const std::string &query) {
    scmd::future future(cass_session_prepare(_session, query.c_str()));
    return future.get_prepared_raw();
}

future session::execute_async(const statement &statement) {
    return future(cass_session_execute(_session, statement.get_statement()));
}
future session::execute_async(statement &&statement) {
    return future(cass_session_execute(_session, statement.get_statement()));
}

future session::execute_async(const std::string &query) {
    return execute_async(statement(query));
}
future session::execute_async(std::string &&query) {
    return execute_async(statement(std::move(query)));
}

future session::execute_async(const batch_query &query) {
    return future(cass_session_execute_batch(_session, query.get_query()));
}
future session::execute_async(batch_query &&query) {
    return future(cass_session_execute_batch(_session, query.get_query()));
}

future session::execute_async(const prepared_query &query) {
    return execute_async(query.get_statement());
}
future session::execute_async(prepared_query &&query) {
    return execute_async(query.get_statement());
}


query_result session::execute(const statement &statement) {
    return execute_async(statement).get_result();
}
query_result session::execute(statement &&statement) {
    return execute_async(std::move(statement)).get_result();
}


query_result session::execute(const std::string &query) {
    return execute_async(query).get_result();
}
query_result session::execute(std::string &&query) {
    return execute_async(std::move(query)).get_result();
}

query_result session::execute(const batch_query &query) {
    return execute_async(query).get_result();
}
query_result session::execute(batch_query &&query) {
    return execute_async(std::move(query)).get_result();
}

query_result session::execute(const prepared_query &query) {
    return execute(query.get_statement());
}
query_result session::execute(prepared_query &&query) {
    return execute(query.get_statement());
}

}  // namespace scmd

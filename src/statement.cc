#include "statement.hh"
#include "cassandra.h"

namespace scmd {
statement::statement(CassStatement *stmt) : _stmt(stmt) {}

statement::statement(const std::string &query, size_t arg_count) {
    _stmt = cass_statement_new(query.c_str(), arg_count);
    // TODO: set_consistency method somewhere else
    cass_statement_set_consistency(_stmt, CASS_CONSISTENCY_QUORUM);
}

statement::statement(const std::string &query) : statement(query, 0) {}

statement::statement(std::string &&query, size_t arg_count) {
    _stmt = cass_statement_new(query.c_str(), arg_count);
    // TODO: set_consistency method somewhere else
    cass_statement_set_consistency(_stmt, CASS_CONSISTENCY_QUORUM);
}

statement::statement(std::string &&query) : statement(std::move(query), 0) {}

statement::statement(statement &&other) noexcept
    : _stmt(std::exchange(other._stmt, nullptr)), _bind_idx(std::exchange(other._bind_idx, 0)) {}

statement &statement::operator=(statement &&other) noexcept {
    std::swap(_stmt, other._stmt);
    std::swap(_bind_idx, other._bind_idx);
    return *this;
}


statement::~statement() {
    cass_statement_free(_stmt);
}

const CassStatement *statement::get_statement() const{
    return _stmt;
}

CassStatement *statement::get_statement() {
    return _stmt;
}

statement &statement::set_timeout(uint64_t timeout) {
    scmd_internal::throw_on_cass_error(cass_statement_set_request_timeout(this->_stmt, timeout));
    return *this;
}

statement &statement::reset(size_t arg_count) {
    scmd_internal::throw_on_cass_error(cass_statement_reset_parameters(_stmt, arg_count));
    return *this;
}
}  // namespace scmd

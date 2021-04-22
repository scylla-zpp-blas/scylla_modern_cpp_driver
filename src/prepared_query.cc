#include "prepared_query.hh"
#include "cassandra.h"

namespace scmd {
prepared_query::prepared_query(const CassPrepared *prepared) : _prepared(prepared) {}

scmd::statement prepared_query::get_statement() const {
    CassStatement *stmt = cass_prepared_bind(_prepared);
    cass_statement_set_consistency(stmt, CASS_CONSISTENCY_QUORUM);

    return scmd::statement(stmt);
}

prepared_query::prepared_query(prepared_query &&other) noexcept : _prepared(std::exchange(other._prepared, nullptr)) {}

prepared_query &prepared_query::operator=(prepared_query &&other) noexcept {
    std::swap(_prepared, other._prepared);
    return *this;
}
prepared_query::~prepared_query() {
    if(_prepared) {
        cass_prepared_free(_prepared);
    }
}

}  // namespace scmd

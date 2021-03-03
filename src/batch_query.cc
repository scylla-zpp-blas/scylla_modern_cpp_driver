#include "batch_query.hh"
#include "exceptions.hh"

scmd::batch_query::batch_query(CassBatchType type) {
    this->_batch = cass_batch_new(type);
}

scmd::batch_query::batch_query(scmd::batch_query &&other) noexcept
    : _batch(std::exchange(other._batch, nullptr)) {}

scmd::batch_query &scmd::batch_query::operator=(scmd::batch_query &&other) noexcept {
    std::swap(other._batch, this->_batch);
    return *this;
}

scmd::batch_query::~batch_query() {
    cass_batch_free(this->_batch);
}

const CassBatch *scmd::batch_query::get_query() const {
    return this->_batch;
}

void scmd::batch_query::set_keyspace(const std::string &keyspace) {
    scmd_internal::throw_on_cass_error(cass_batch_set_keyspace_n(this->_batch, keyspace.c_str(), keyspace.length()));
}

void scmd::batch_query::set_consistency(CassConsistency consistency) {
    scmd_internal::throw_on_cass_error(cass_batch_set_consistency(this->_batch, consistency));
}

void scmd::batch_query::set_serial_consistency(CassConsistency consistency) {
    scmd_internal::throw_on_cass_error(cass_batch_set_serial_consistency(this->_batch, consistency));
}

void scmd::batch_query::set_timestamp(int64_t timestamp) {
    scmd_internal::throw_on_cass_error(cass_batch_set_timestamp(this->_batch, timestamp));
}

void scmd::batch_query::add_statement(scmd::statement &statement) {
    scmd_internal::throw_on_cass_error(cass_batch_add_statement(this->_batch, statement.get_statement()));
}


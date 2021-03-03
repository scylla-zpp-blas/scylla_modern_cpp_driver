#include <algorithm>


#include "exceptions.hh"
#include "future.hh"
#include "prepared_query.hh"

scmd::future::future(CassFuture *future) {
    this->_future = future;
}

scmd::future::future(scmd::future &&other) noexcept
    : _future(std::exchange(other._future, nullptr)) {}

scmd::future &scmd::future::operator=(scmd::future &&other) noexcept {
    std::swap(other._future, this->_future);
    return *this;
}

scmd::future::~future() {
    cass_future_free(this->_future);
}

bool scmd::future::is_ready() {
    return cass_future_ready(this->_future) == cass_true;
}

void scmd::future::wait() {
    cass_future_wait(this->_future);
}

bool scmd::future::wait(uint64_t ms) {
    return cass_future_wait_timed(this->_future, ms);
}

scmd::query_result scmd::future::get_result() {
    const CassResult *result = cass_future_get_result(this->_future);
    if(result == nullptr) {
        //TODO: Maybe throw error result and/or error message?
        scmd_internal::throw_on_cass_error(cass_future_error_code(this->_future));
    }
    return scmd::query_result(result);
}

scmd::prepared_query scmd::future::get_prepared() {
    const CassPrepared *result = cass_future_get_prepared(this->_future);
    if(result == nullptr) {
        //TODO: Maybe throw error result and/or error message?
        scmd_internal::throw_on_cass_error(cass_future_error_code(this->_future));
    }
    return scmd::prepared_query(result);
}


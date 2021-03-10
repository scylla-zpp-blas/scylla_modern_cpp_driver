#include <algorithm>
#include <utility>


#include "exceptions.hh"
#include "future.hh"
#include "prepared_query.hh"

scmd::future::future(CassFuture *future) :
      _future(future) {
    cb = new scmd_internal::future_callback(this);
}


scmd::future::future(scmd::future &&other) noexcept
    : _future(std::exchange(other._future, nullptr)),
      cb(std::exchange(other.cb, nullptr)) {
    this->cb->future = this;
}

scmd::future &scmd::future::operator=(scmd::future &&other) noexcept {
    std::swap(other._future, this->_future);
    std::swap(other.cb, this->cb);
    std::swap(this->cb->future, other.cb->future);
    return *this;
}

scmd::future::~future() {
    cass_future_free(this->_future);
    delete cb;
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
    if (result == nullptr) {
        // TODO: Maybe throw error result and/or error message?
        scmd_internal::throw_on_cass_error(cass_future_error_code(this->_future));
    }
    return scmd::prepared_query(result);
}

void scmd::future::set_callback_fast(scmd::future::callback_type_fast f) {
    this->cb->set_callback_fast(f);
    scmd_internal::throw_on_cass_error(cass_future_set_callback(this->_future, scmd_internal::future_callback::callback_fn_fast, static_cast<void*>(this->cb)));
}

void scmd::future::set_callback_fast(scmd::future::callback_type_fast_bound f, void *arg) {
    this->cb->set_callback_fast(f, arg);
    scmd_internal::throw_on_cass_error(cass_future_set_callback(this->_future, scmd_internal::future_callback::callback_fn_fast_bound, static_cast<void*>(this->cb)));
}

void scmd::future::set_callback(const callback_type_universal &f) {
    this->cb->set_callback(f);
    scmd_internal::throw_on_cass_error(cass_future_set_callback(this->_future, scmd_internal::future_callback::callback_fn, static_cast<void*>(this->cb)));
}


#pragma once

#include <functional>

#include "cassandra.h"
#include "prepared_query.hh"
#include "query_result.hh"
#include "future_callback.hh"

namespace scmd {
class future {
    CassFuture *_future;
public:
    using callback_type_fast = scmd_internal::callback_type_fast;
    using callback_type_fast_bound = scmd_internal::callback_type_fast_bound;
    using callback_type_universal = scmd_internal::callback_type_univeral;

    explicit future(CassFuture *future);

    // We can't really copy this class.
    future(const future &other) = delete;
    future &operator=(const future &other) = delete;

    future(future &&other) noexcept;
    future &operator=(future &&other) noexcept;

    ~future();

    bool is_ready();

    void wait();

    bool wait(uint64_t ms);

    query_result get_result();

    prepared_query get_prepared();

    /* IMPORTANT WARNING */
    // After setting callback, the object must not be destroyed
    // until after callback is called.
    // If you move-construct other future using future with callback set, both new and old object
    // must not be destroyed until after callback is called.
    void set_callback_fast(callback_type_fast f);

    void set_callback_fast(callback_type_fast_bound f, void *arg);

    void set_callback(const std::function<void(scmd::future*)>& f);

private:
    std::unique_ptr<scmd_internal::future_callback> cb;
};
}  // namespace scmd


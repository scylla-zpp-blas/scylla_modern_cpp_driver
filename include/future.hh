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
    using callback_type_universal = scmd_internal::callback_type_universal;

    explicit future(CassFuture *future);

    // We can't really copy this class.
    future(const future &other) = delete;
    future &operator=(const future &other) = delete;

    future(future &&other) noexcept;
    future &operator=(future &&other) noexcept;

    ~future();

    bool is_ready();

    void throw_errors();

    void wait();

    bool wait(uint64_t ms);

    query_result get_result();

    prepared_query get_prepared();

    void set_callback_fast(callback_type_fast f);

    void set_callback_fast(callback_type_fast_bound f, void *arg);

    void set_callback(const callback_type_universal &f);

private:
    scmd_internal::future_callback *cb;
};
}  // namespace scmd


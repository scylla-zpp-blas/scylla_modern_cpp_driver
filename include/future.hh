#pragma once

#include <functional>

#include "cassandra.h"
#include "prepared_query.hh"
#include "query_result.hh"

namespace scmd {
class future {
    CassFuture *_future;
public:
    using callback_type_fast = void(*) (future *future);
    using callback_type_fast_bound = void(*) (future *future, void *arg);

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

    void set_callback_fast(callback_type_fast f);

    void set_callback_fast(callback_type_fast_bound f, void *arg);

    void set_callback(const std::function<void(scmd::future*)>& f);

private:
    class callback_struct {
    public:
        scmd::future *future;
        std::function<void(scmd::future*)> fn = nullptr;
        union {
            scmd::future::callback_type_fast fn_fast;
            struct {
                scmd::future::callback_type_fast_bound fn_fast_bound;
                void *arg;
            };
        };

        explicit callback_struct(scmd::future *f) : future(f) {};

        void set_callback(const std::function<void(scmd::future*)> &fn) {
            this->fn = fn;
        }

        void set_callback(scmd::future::callback_type_fast fn_fast) {
            this->fn_fast = fn_fast;
        }

        void set_callback(scmd::future::callback_type_fast_bound fn_fast_bound, void *arg) {
            this->fn_fast_bound = fn_fast_bound;
            this->arg = arg;
        }
    };

    callback_struct cb{this};

    static const inline CassFutureCallback callback_fn_fast = [](CassFuture *future, void *data) {
      static_cast<scmd::future::callback_struct*>(data)->fn_fast(static_cast<callback_struct*>(data)->future);
    };

    static const inline CassFutureCallback callback_fn_fast_bound = [](CassFuture *future, void *data) {
      static_cast<scmd::future::callback_struct*>(data)->fn_fast_bound(
            static_cast<callback_struct*>(data)->future,
            static_cast<callback_struct*>(data)->arg);
    };

    static const inline CassFutureCallback callback_fn = [](CassFuture *future, void *data) {
        static_cast<scmd::future::callback_struct*>(data)->fn(static_cast<callback_struct*>(data)->future);
    };
};
}  // namespace scmd

#pragma once

#include <functional>

#include "cassandra.h"

namespace scmd {
class future;
}

namespace scmd_internal {

using callback_type_fast = void(*) (scmd::future *future);
using callback_type_fast_bound = void(*) (scmd::future *future, void *arg);
using callback_type_univeral = std::function<void(scmd::future *)>;

class future_callback {
public:
    scmd::future *future;
    std::function<void(scmd::future *)> fn = nullptr;
    union {
        callback_type_fast fn_fast;
        struct {
            callback_type_fast_bound fn_fast_bound = nullptr;
            void *arg = nullptr;
        };
    };

    explicit future_callback(scmd::future *f) : future(f){};

    void set_callback(const std::function<void(scmd::future *)> &fn);

    void set_callback_fast(callback_type_fast fn_fast);

    void set_callback_fast(callback_type_fast_bound fn_fast_bound, void *arg);

    static CassFutureCallback callback_fn;

    static CassFutureCallback callback_fn_fast;

    static CassFutureCallback callback_fn_fast_bound;
};

}
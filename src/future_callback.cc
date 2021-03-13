#include "future_callback.hh"

namespace scmd_internal {
void future_callback::set_callback(const callback_type_universal &fn) {
    this->fn = fn;
}

void future_callback::set_callback_fast(callback_type_fast fn_fast) {
    this->fn_fast = fn_fast;
}

void future_callback::set_callback_fast(callback_type_fast_bound fn_fast_bound, void *arg) {
    this->fn_fast_bound = fn_fast_bound;
    this->arg = arg;
}

CassFutureCallback future_callback::callback_fn_fast = [](CassFuture *_, void *data) {
    static_cast<future_callback *>(data)->fn_fast(static_cast<future_callback *>(data)->future);
};


CassFutureCallback future_callback::callback_fn_fast_bound = [](CassFuture *future, void *data) {
    static_cast<future_callback *>(data)->fn_fast_bound(static_cast<future_callback *>(data)->future,
                                                        static_cast<future_callback *>(data)->arg);
};

CassFutureCallback future_callback::callback_fn = [](CassFuture *future, void *data) {
    static_cast<future_callback *>(data)->fn(static_cast<future_callback *>(data)->future);
};
}
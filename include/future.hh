#pragma once
#include "cassandra.h"
#include "prepared_query.hh"
#include "query_result.hh"

namespace scmd {
class future {
    CassFuture *_future;
public:
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
};
}  // namespace scmd

#pragma once

#include <stdexcept>
#include <string>

#include "cassandra.h"

namespace scmd {
class exception : public std::runtime_error {
    CassError _rc;

public:
    exception(CassError rc);

    CassError get_error();
};

}  // namespace scmd

namespace scmd_internal {
void throw_on_cass_error(CassError rc);

template <typename Func>
void throw_on_cass_error(CassError rc, Func cleanup_func) {
    if (rc != CASS_OK) {
        cleanup_func();
        throw scmd::exception(rc);
    }
}
}  // namespace scmd_internal

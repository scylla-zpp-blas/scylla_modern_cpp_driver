#pragma once

#include <stdexcept>
#include <string>

#include "cassandra.h"

namespace scmd {
class exception : public std::runtime_error {
    CassError _rc;

public:
    exception(CassError rc);
    exception(CassError rc, const std::string& msg);

    CassError get_error();
};

}  // namespace scmd

namespace scmd_internal {
void throw_on_cass_error(CassError rc);
void throw_on_cass_error(CassError rc);

template <typename Func>
void throw_on_cass_error(CassError rc, Func cleanup_func) {
    if (rc != CASS_OK) {
        cleanup_func();
        throw scmd::exception(rc);
    }
}

template <typename Func>
void throw_on_cass_error(CassError rc, const std::string& s, Func cleanup_func) {
    if (rc != CASS_OK) {
        cleanup_func();
        throw scmd::exception(rc, s);
    }
}
}  // namespace scmd_internal

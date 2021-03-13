#include "exceptions.hh"
#include "cassandra.h"

namespace scmd {
using namespace std::string_literals;
exception::exception(CassError rc)
    : runtime_error("scylla driver exception: "s + cass_error_desc(rc) + ".\n"
                    "Error code: " + std::to_string(rc)),
      _rc(rc) {}

exception::exception(CassError rc, const std::string& msg)
    : runtime_error("scylla driver exception: "s + cass_error_desc(rc) + ".\n"
                    "Error code: " + std::to_string(rc) + ".\n"
                    "Message: " + msg),
      _rc(rc) {}
CassError exception::get_error() {
    return _rc;
}

}  // namespace scmd

namespace scmd_internal {
void throw_on_cass_error(CassError rc) {
    if (rc != CASS_OK) {
        throw scmd::exception(rc);
    }
}

void throw_on_cass_error(CassError rc, const std::string& s) {
    if (rc != CASS_OK) {
        throw scmd::exception(rc, s);
    }
}
}  // namespace scmd_internal

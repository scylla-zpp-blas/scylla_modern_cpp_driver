#include "cassandra.h"
#include "exceptions.hh"
#include "fmt/format.h"

namespace scmd {
    exception::exception(CassError rc) :
            runtime_error(fmt::format("scylla driver exception: {}. Error code: {}", cass_error_desc(rc), rc)),
            _rc(rc) {}

    CassError exception::get_error() {
        return _rc;
    }


}

namespace scmd_internal {
    void throw_on_cass_error(CassError rc) {
        if (rc != CASS_OK) {
            throw scmd::exception(rc);
        }
    }
}


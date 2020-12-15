#include "cassandra.h"
#include "value_converters/statement_binder.hh"

namespace scmd_internal {

    template<>
    void statement_binder_helper<int8_t>::bind_to_statement(CassStatement* statement, size_t index, int8_t value) {
        throw_on_cass_error(cass_statement_bind_int8(statement, index, value));
    }


    template<>
    void statement_binder_helper<int16_t>::bind_to_statement(CassStatement* statement, size_t index, int16_t value) {
        throw_on_cass_error(cass_statement_bind_int16(statement, index, value));
    }


    template<>
    void statement_binder_helper<int32_t>::bind_to_statement(CassStatement* statement, size_t index, int32_t value) {
        throw_on_cass_error(cass_statement_bind_int32(statement, index, value));
    }


    template<>
    void statement_binder_helper<uint32_t>::bind_to_statement(CassStatement* statement, size_t index, uint32_t value) {
        throw_on_cass_error(cass_statement_bind_uint32(statement, index, value));
    }


    template<>
    void statement_binder_helper<int64_t>::bind_to_statement(CassStatement* statement, size_t index, int64_t value) {
        throw_on_cass_error(cass_statement_bind_int64(statement, index, value));
    }


    template<>
    void statement_binder_helper<float>::bind_to_statement(CassStatement* statement, size_t index, float value) {
        throw_on_cass_error(cass_statement_bind_float(statement, index, value));
    }

    template<>
    void statement_binder_helper<double>::bind_to_statement(CassStatement* statement, size_t index, double value) {
        throw_on_cass_error(cass_statement_bind_double(statement, index, value));
    }


    template<>
    void statement_binder_helper<const std::string&>::bind_to_statement(CassStatement* statement, size_t index, const std::string& value) {
        throw_on_cass_error(cass_statement_bind_string(statement, index, value.c_str()));
    }

    template<>
    void statement_binder_helper<CassCollection *>::bind_to_statement(CassStatement* statement, size_t index, CassCollection * value) {
        throw_on_cass_error(cass_statement_bind_collection(statement, index, value));
    }
}
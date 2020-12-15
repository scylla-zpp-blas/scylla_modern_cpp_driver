#include "cassandra.h"
#include "value_converters/tuple_binder.hh"


namespace scmd_internal {
    template<>
    void tuple_binder_helper<int8_t>::bind_to_tuple(CassTuple* tuple, size_t index, int8_t value) {
        throw_on_cass_error(cass_tuple_set_int8(tuple, index, value));
    }


    template<>
    void tuple_binder_helper<int16_t>::bind_to_tuple(CassTuple* tuple, size_t index, int16_t value) {
        throw_on_cass_error(cass_tuple_set_int16(tuple, index, value));
    }


    template<>
    void tuple_binder_helper<int32_t>::bind_to_tuple(CassTuple* tuple, size_t index, int32_t value) {
        throw_on_cass_error(cass_tuple_set_int32(tuple, index, value));
    }


    template<>
    void tuple_binder_helper<uint32_t>::bind_to_tuple(CassTuple* tuple, size_t index, uint32_t value) {
        throw_on_cass_error(cass_tuple_set_uint32(tuple, index, value));
    }


    template<>
    void tuple_binder_helper<int64_t>::bind_to_tuple(CassTuple* tuple, size_t index, int64_t value) {
        throw_on_cass_error(cass_tuple_set_int64(tuple, index, value));
    }


    template<>
    void tuple_binder_helper<float>::bind_to_tuple(CassTuple* tuple, size_t index, float value) {
        throw_on_cass_error(cass_tuple_set_float(tuple, index, value));
    }

    template<>
    void tuple_binder_helper<double>::bind_to_tuple(CassTuple* tuple, size_t index, double value) {
        throw_on_cass_error(cass_tuple_set_double(tuple, index, value));
    }


    template<>
    void tuple_binder_helper<const std::string&>::bind_to_tuple(CassTuple* tuple, size_t index, const std::string& value) {
        throw_on_cass_error(cass_tuple_set_string(tuple, index, value.c_str()));
    }

    template<>
    void tuple_binder_helper<CassCollection *>::bind_to_tuple(CassTuple* tuple, size_t index, CassCollection * value) {
        throw_on_cass_error(cass_tuple_set_collection(tuple, index, value));
    }
}
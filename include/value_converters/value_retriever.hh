#pragma once

#include <string>

#include "cassandra.h"
#include "exceptions.hh"

namespace scmd_internal {
// TODO: implement uuid, inet, bytes, decimal, duration, dse_date_range, dse_point
template <typename T>
T retrieve_value(const CassValue *value) ;

template <>
inline bool retrieve_value<bool>(const CassValue *value) {
    cass_bool_t val;
    throw_on_cass_error(cass_value_get_bool(value, &val));
    return val == cass_bool_t::cass_true;
}

template <>
inline int8_t retrieve_value<int8_t>(const CassValue *value) {
    int8_t ret;
    throw_on_cass_error(cass_value_get_int8(value, &ret));
    return ret;
}

template <>
inline int16_t retrieve_value<int16_t>(const CassValue *value) {
    int16_t ret;
    throw_on_cass_error(cass_value_get_int16(value, &ret));
    return ret;
}

template <>
inline int32_t retrieve_value<int32_t>(const CassValue *value) {
    int32_t ret;
    throw_on_cass_error(cass_value_get_int32(value, &ret));
    return ret;
}

template <>
inline uint32_t retrieve_value<uint32_t>(const CassValue *value) {
    uint32_t ret;
    throw_on_cass_error(cass_value_get_uint32(value, &ret));
    return ret;
}

template <>
inline int64_t retrieve_value<int64_t>(const CassValue *value) {
    int64_t ret;
    throw_on_cass_error(cass_value_get_int64(value, &ret));
    return ret;
}

template <>
inline float retrieve_value<float>(const CassValue *value) {
    float ret;
    throw_on_cass_error(cass_value_get_float(value, &ret));
    return ret;
}

template <>
inline double retrieve_value<double>(const CassValue *value) {
    double ret;
    throw_on_cass_error(cass_value_get_double(value, &ret));
    return ret;
}

template <>
inline std::string retrieve_value<std::string>(const CassValue *value) {
    const char *str;
    size_t size;
    throw_on_cass_error(cass_value_get_string(value, &str, &size));
    return std::string(str, size);
}

inline bool is_cass_value_null(const CassValue *value) {
    return cass_value_is_null(value) == cass_true;
}

}  // namespace scmd_internal

#include "value_converters/value_retriever.hh"
#include "exceptions.hh"

namespace scmd_internal {
template <>
int8_t retrieve_value<int8_t>(const CassValue *value) {
    int8_t ret;
    throw_on_cass_error(cass_value_get_int8(value, &ret));
    return ret;
}

template <>
int16_t retrieve_value<int16_t>(const CassValue *value) {
    int16_t ret;
    throw_on_cass_error(cass_value_get_int16(value, &ret));
    return ret;
}

template <>
int32_t retrieve_value<int32_t>(const CassValue *value) {
    int32_t ret;
    throw_on_cass_error(cass_value_get_int32(value, &ret));
    return ret;
}

template <>
uint32_t retrieve_value<uint32_t>(const CassValue *value) {
    uint32_t ret;
    throw_on_cass_error(cass_value_get_uint32(value, &ret));
    return ret;
}

template <>
int64_t retrieve_value<int64_t>(const CassValue *value) {
    int64_t ret;
    throw_on_cass_error(cass_value_get_int64(value, &ret));
    return ret;
}

template <>
float retrieve_value<float>(const CassValue *value) {
    float ret;
    throw_on_cass_error(cass_value_get_float(value, &ret));
    return ret;
}

template <>
double retrieve_value<double>(const CassValue *value) {
    double ret;
    throw_on_cass_error(cass_value_get_double(value, &ret));
    return ret;
}

template <>
std::string retrieve_value<std::string>(const CassValue *value) {
    const char *str;
    size_t size;
    throw_on_cass_error(cass_value_get_string(value, &str, &size));
    return std::string(str, size);
}

bool is_cass_value_null(const CassValue *value) {
    return cass_value_is_null(value) == cass_true;
}
}  // namespace scmd_internal
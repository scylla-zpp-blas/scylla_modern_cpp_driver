#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

#include "cassandra.h"

#include "exceptions.hh"
#include "tuple_binder.hh"

namespace scmd_internal {
// TODO: append null, bytes, custom, uuid, inet, decimal, duration, user_type
// TODO: handle not only std:: collections, but everything that's interface-compatibile with collection
template <typename T>
struct collection_appender_helper {
    static void append_to_collection(CassCollection *collection, T value);
};

template <typename... Types>
struct collection_appender_helper<std::tuple<Types...>> {
    static void append_to_collection(CassCollection *collection, const std::tuple<Types...> &value) {
        CassTuple *t = cass_tuple_from_tuple(value);
        auto cleanup = [=]() { cass_tuple_free(t); };
        throw_on_cass_error(cass_collection_append_tuple(collection, t), cleanup);
        cleanup();
    }
};

template <>
inline void collection_appender_helper<bool>::append_to_collection(CassCollection *collection, bool value) {
    throw_on_cass_error(cass_collection_append_bool(collection, value ? cass_bool_t::cass_true : cass_bool_t::cass_false));
}

template <>
inline void collection_appender_helper<int8_t>::append_to_collection(CassCollection *collection, int8_t value) {
    throw_on_cass_error(cass_collection_append_int8(collection, value));
}

template <>
inline void collection_appender_helper<int16_t>::append_to_collection(CassCollection *collection, int16_t value) {
    throw_on_cass_error(cass_collection_append_int16(collection, value));
}

template <>
inline void collection_appender_helper<int32_t>::append_to_collection(CassCollection *collection, int32_t value) {
    throw_on_cass_error(cass_collection_append_int32(collection, value));
}

template <>
inline void collection_appender_helper<uint32_t>::append_to_collection(CassCollection *collection, uint32_t value) {
    throw_on_cass_error(cass_collection_append_uint32(collection, value));
}

template <>
inline void collection_appender_helper<int64_t>::append_to_collection(CassCollection *collection, int64_t value) {
    throw_on_cass_error(cass_collection_append_int64(collection, value));
}

template <>
inline void collection_appender_helper<float>::append_to_collection(CassCollection *collection, float value) {
    throw_on_cass_error(cass_collection_append_float(collection, value));
}

template <>
inline void collection_appender_helper<double>::append_to_collection(CassCollection *collection, double value) {
    throw_on_cass_error(cass_collection_append_double(collection, value));
}

template <>
inline void collection_appender_helper<std::string>::append_to_collection(CassCollection *collection,
                                                                   std::string value) {
    throw_on_cass_error(cass_collection_append_string(collection, value.c_str()));
}

template <>
inline void collection_appender_helper<CassCollection *>::append_to_collection(CassCollection *collection,
                                                                        CassCollection *value) {
    throw_on_cass_error(cass_collection_append_collection(collection, value));
}

template <typename T>
void append_to_collection(CassCollection *collection, T value) {
    collection_appender_helper<T>::append_to_collection(collection, value);
}
}  // namespace scmd_internal

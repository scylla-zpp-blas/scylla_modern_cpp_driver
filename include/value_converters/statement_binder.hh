#pragma once

#include <cstddef>
#include <cstdint>
#include <set>
#include <string>
#include <vector>

#include "cassandra.h"

#include "collection_appender.hh"
#include "exceptions.hh"

namespace scmd_internal {
// TODO: bind null, bytes, custom, uuid, inet, decimal, duration, user_type
template <typename T>
struct statement_binder_helper {
    static void bind_to_statement(CassStatement *statement, size_t index, T value);
};

template <typename... Types>
struct statement_binder_helper<std::tuple<Types...>> {
    static void bind_to_statement(CassStatement *statement, size_t index, const std::tuple<Types...> &value) {
        CassTuple *t = cass_tuple_from_tuple(value);
        auto cleanup = [=]() { cass_tuple_free(t); };
        throw_on_cass_error(cass_statement_bind_tuple(statement, index, t), cleanup);
        cleanup();
    }
};

template <typename Type>
struct statement_binder_helper<std::set<Type>> {
    static void bind_to_statement(CassStatement *statement, size_t index, const std::set<Type> &value) {
        CassCollection *set = cass_collection_new(CASS_COLLECTION_TYPE_SET, value.size());
        auto cleanup = [=]() { cass_collection_free(set); };
        for (const auto &it : value) {
            try {
                append_to_collection(set, it);
            } catch (const scmd::exception &e) {
                cleanup();
                throw e;
            }
        }
        cass_statement_bind_collection(statement, index, set);
        cleanup();
    }
};

template <typename Type>
struct statement_binder_helper<std::vector<Type>> {
    static void bind_to_statement(CassStatement *statement, size_t index, const std::vector<Type> &value) {
        CassCollection *set = cass_collection_new(CASS_COLLECTION_TYPE_SET, value.size());
        auto cleanup = [=]() { cass_collection_free(set); };
        for (const auto &it : value) {
            try {
                append_to_collection(set, it);
            } catch (const scmd::exception &e) {
                cleanup();
                throw e;
            }
        }
        cass_statement_bind_collection(statement, index, set);
        cleanup();
    }
};

template <>
inline void statement_binder_helper<bool>::bind_to_statement(CassStatement *statement, size_t index, bool value) {
    throw_on_cass_error(cass_statement_bind_bool(statement, index, value ? cass_bool_t::cass_true : cass_bool_t::cass_false));
}

template <>
inline void statement_binder_helper<int8_t>::bind_to_statement(CassStatement *statement, size_t index, int8_t value) {
    throw_on_cass_error(cass_statement_bind_int8(statement, index, value));
}

template <>
inline void statement_binder_helper<int16_t>::bind_to_statement(CassStatement *statement, size_t index, int16_t value) {
    throw_on_cass_error(cass_statement_bind_int16(statement, index, value));
}

template <>
inline void statement_binder_helper<int32_t>::bind_to_statement(CassStatement *statement, size_t index, int32_t value) {
    throw_on_cass_error(cass_statement_bind_int32(statement, index, value));
}

template <>
inline void statement_binder_helper<uint32_t>::bind_to_statement(CassStatement *statement, size_t index, uint32_t value) {
    throw_on_cass_error(cass_statement_bind_uint32(statement, index, value));
}

template <>
inline void statement_binder_helper<int64_t>::bind_to_statement(CassStatement *statement, size_t index, int64_t value) {
    throw_on_cass_error(cass_statement_bind_int64(statement, index, value));
}

template <>
inline void statement_binder_helper<float>::bind_to_statement(CassStatement *statement, size_t index, float value) {
    throw_on_cass_error(cass_statement_bind_float(statement, index, value));
}

template <>
inline void statement_binder_helper<double>::bind_to_statement(CassStatement *statement, size_t index, double value) {
    throw_on_cass_error(cass_statement_bind_double(statement, index, value));
}

template <>
inline void statement_binder_helper<std::string>::bind_to_statement(CassStatement *statement, size_t index,
                                                             std::string value) {
    throw_on_cass_error(cass_statement_bind_string(statement, index, value.c_str()));
}

template <>
inline void statement_binder_helper<CassCollection *>::bind_to_statement(CassStatement *statement, size_t index,
                                                                  CassCollection *value) {
    throw_on_cass_error(cass_statement_bind_collection(statement, index, value));
}

template <typename T>
void bind_to_statement(CassStatement *statement, size_t index, T value) {
    scmd_internal::statement_binder_helper<T>::bind_to_statement(statement, index, value);
}

}  // namespace scmd_internal

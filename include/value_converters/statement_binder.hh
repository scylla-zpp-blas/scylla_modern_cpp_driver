#pragma once

#include <cstddef>
#include <cstdint>
#include <set>
#include <string>
#include <vector>

#include "cassandra.h"

#include "collection_appender.hh"
#include "exceptions.hh"
#include "traits.hh"

namespace scmd_internal {
// TODO: bind null, bool, bytes, custom, uuid, inet, decimal, duration, user_type
template <typename T,
          typename = typename std::enable_if<scmd_internal::is_statement_binder_implemented_simple<T>::value>::type>
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

template <typename T>
void bind_to_statement(CassStatement *statement, size_t index, T value) {
    scmd_internal::statement_binder_helper<T>::bind_to_statement(statement, index, value);
}
}  // namespace scmd_internal

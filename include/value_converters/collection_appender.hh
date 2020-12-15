#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

#include "cassandra.h"

#include "tuple_binder.hh"
#include "exceptions.hh"

namespace scmd_internal {
    // TODO: append null, bool, bytes, custom, uuid, inet, decimal, duration, user_type
    // TODO: handle not only std:: collections, but everything that's interface-compatibile with collection
    template<typename T, typename = typename std::enable_if<scmd_internal::is_collection_appender_implemented_simple<T>::value>::type>
    struct collection_appender_helper {
        static void append_to_collection(CassCollection* collection, T value);
    };


    template<typename... Types>
    struct collection_appender_helper<std::tuple<Types...>> {
        static void append_to_collection(CassCollection* collection, const std::tuple<Types...>& value) {
            CassTuple *t = cass_tuple_from_tuple(value);
            auto cleanup = [=]() { cass_tuple_free(t); };
            throw_on_cass_error(cass_collection_append_tuple(collection, t), cleanup);
            cleanup();
        }
    };

    template<typename T>
    void append_to_collection(CassCollection* collection, T value) {
        collection_appender_helper<T>::append_to_collection(collection, value);
    }
}



#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <tuple>
#include <type_traits>

#include "cassandra.h"

#include "exceptions.hh"
#include "traits.hh"

namespace scmd_internal {
    // TODO: bind null, bool, bytes, custom, uuid, inet, decimal, duration, user_type
    // TODO: handle not only std::tuple, but everything that's interface-compatibile with tuple
    template<typename T, typename = typename std::enable_if<scmd_internal::is_tuple_binder_implemented_simple<T>::value>::type>
    struct tuple_binder_helper {
        static void bind_to_tuple(CassTuple* tuple, size_t index, T value);
    };


    template<typename... Types>
    struct tuple_binder_helper<std::tuple<Types...>> {
        static void bind_to_tuple(CassTuple* tuple, size_t index, const std::tuple<Types...>& value) {
            CassTuple *t = cass_tuple_from_tuple(value);
            auto cleanup = [=]() { cass_tuple_free(t); };
            throw_on_cass_error(cass_tuple_set_tuple(tuple, index, t), cleanup);
            cleanup();
        }
    };


    template<std::size_t I = 0, typename... Types>
    inline typename std::enable_if<I == sizeof...(Types), void>::type
    tuple_binder_iterator(CassTuple *t, const std::tuple<Types...>& value)
    { }

    template<std::size_t I = 0, typename... Types>
    inline typename std::enable_if<I < sizeof...(Types), void>::type
    tuple_binder_iterator(CassTuple *t, const std::tuple<Types...>& value)
    {
        bind_to_tuple(t, I, std::get<I>(value));
        tuple_binder_iterator<I + 1, Types...>(t, value);
    }

    template<typename... Types>
    CassTuple *cass_tuple_from_tuple(const std::tuple<Types...>& value) {
        CassTuple *t = cass_tuple_new(sizeof...(Types));
        auto cleanup = [=]() { cass_tuple_free(t); };
        try {
            tuple_binder_iterator(t, value);
        }catch (const scmd::exception& e) {
            cleanup();
            throw e;
        }

        return t;
    }

    template<typename T>
    void bind_to_tuple(CassTuple* tuple, size_t index, T value) {
        scmd_internal::tuple_binder_helper<T>::bind_to_tuple(tuple, index, value);
    }
}





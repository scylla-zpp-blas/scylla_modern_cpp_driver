#pragma once

#include <string>

#include "cassandra.h"
#include "traits.hh"

namespace scmd_internal {
    //TODO: implement bool, uuid, inet, bytes, decimal, duration, dse_date_range, dse_point
    template<typename T, typename = typename std::enable_if<scmd_internal::is_tuple_binder_implemented_simple<T>::value>::type>
    T retrieve_value(const CassValue* value);

    bool is_cass_value_null(const CassValue *value);
}

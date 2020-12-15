#include "cassandra.h"
#include "value_converters/collection_appender.hh"
#include "exceptions.hh"

namespace scmd_internal {
    template<>
    void collection_appender_helper<int8_t>::append_to_collection(CassCollection* collection, int8_t value) {
        throw_on_cass_error(cass_collection_append_int8(collection, value));
    }


    template<>
    void collection_appender_helper<int16_t>::append_to_collection(CassCollection* collection, int16_t value) {
        throw_on_cass_error(cass_collection_append_int16(collection, value));
    }


    template<>
    void collection_appender_helper<int32_t>::append_to_collection(CassCollection* collection, int32_t value) {
        throw_on_cass_error(cass_collection_append_int32(collection, value));
    }


    template<>
    void collection_appender_helper<uint32_t>::append_to_collection(CassCollection* collection, uint32_t value) {
        throw_on_cass_error(cass_collection_append_uint32(collection, value));
    }


    template<>
    void collection_appender_helper<int64_t>::append_to_collection(CassCollection* collection, int64_t value) {
        throw_on_cass_error(cass_collection_append_int64(collection, value));
    }


    template<>
    void collection_appender_helper<float>::append_to_collection(CassCollection* collection, float value) {
        throw_on_cass_error(cass_collection_append_float(collection, value));
    }

    template<>
    void collection_appender_helper<double>::append_to_collection(CassCollection* collection, double value) {
        throw_on_cass_error(cass_collection_append_double(collection, value));
    }


    template<>
    void collection_appender_helper<const std::string&>::append_to_collection(CassCollection* collection, const std::string& value) {
        throw_on_cass_error(cass_collection_append_string(collection, value.c_str()));
    }

    template<>
    void collection_appender_helper<CassCollection *>::append_to_collection(CassCollection* collection, CassCollection * value) {
        throw_on_cass_error(cass_collection_append_collection(collection, value));
    }
}
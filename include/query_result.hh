#pragma once

#include <string>

#include "cassandra.h"
#include "value_converters/value_retriever.hh"

namespace scmd {
    class query_result {
        const CassResult* _result;
        CassIterator* _iterator;
        const CassRow* _row;
    public:
        explicit query_result(const CassResult* res);

        // We can't really copy this class
        query_result(const query_result& other) = delete;
        query_result& operator=(const query_result& other) = delete;

        query_result(query_result&& other) noexcept;
        query_result& operator=(query_result&& other) noexcept;

        /* Prepares the next row of the request response.
         * Returns true if successful, false if there are no rows left to process.
         */
        bool next_row();

        /* Get the CassValue for given column in the currently processed row. */
        template<typename T>
        T get_column(const std::string& name) {
            const CassValue *cell = cass_row_get_column_by_name(_row, name.c_str());
            return scmd_internal::retrieve_value<T>(cell);
        }

        template<typename T>
        T get_column(size_t index) {
            const CassValue *cell = cass_row_get_column(_row, index);
            return scmd_internal::retrieve_value<T>(cell);
        }

        const CassValue *get_column_raw(const std::string& name);

        const CassValue *get_column_raw(size_t index);

        bool is_column_null(const std::string& name);
        bool is_column_null(size_t index);

        std::string get_column_name(size_t index);

        ~query_result();
    };

}

#pragma once

#include "cassandra.h"

namespace scmd_internal {
    template<typename Test, template<typename...> class Ref>
    struct is_specialization_of : std::false_type {};

    template<template<typename...> class Ref, typename... Args>
    struct is_specialization_of<Ref<Args...>, Ref>: std::true_type {};

    template<typename T, typename = std::enable_if<
            std::is_same_v<T, int8_t> ||
            std::is_same_v<T, int16_t> ||
            std::is_same_v<T, int32_t> ||
            std::is_same_v<T, uint32_t> ||
            std::is_same_v<T, int64_t> ||
            std::is_same_v<T, float> ||
            std::is_same_v<T, double> ||
            std::is_same_v<T, std::string> ||
            std::is_same_v<T, CassCollection *> >>
    struct is_statement_binder_implemented_simple : std::true_type {};

    template<typename T, typename = std::enable_if<
            std::is_same_v<T, int8_t> ||
            std::is_same_v<T, int16_t> ||
            std::is_same_v<T, int32_t> ||
            std::is_same_v<T, uint32_t> ||
            std::is_same_v<T, int64_t> ||
            std::is_same_v<T, float> ||
            std::is_same_v<T, double> ||
            std::is_same_v<T, std::string> ||
            std::is_same_v<T, CassCollection *> >>
    struct is_tuple_binder_implemented_simple : std::true_type {};

    template<typename T, typename = std::enable_if<
            std::is_same_v<T, int8_t> ||
            std::is_same_v<T, int16_t> ||
            std::is_same_v<T, int32_t> ||
            std::is_same_v<T, uint32_t> ||
            std::is_same_v<T, int64_t> ||
            std::is_same_v<T, float> ||
            std::is_same_v<T, double> ||
            std::is_same_v<T, std::string> ||
            std::is_same_v<T, CassCollection *> >>
    struct is_collection_appender_implemented_simple : std::true_type {};


    template<typename T, typename = std::enable_if<
            std::is_same_v<T, int8_t> ||
            std::is_same_v<T, int16_t> ||
            std::is_same_v<T, int32_t> ||
            std::is_same_v<T, uint32_t> ||
            std::is_same_v<T, int64_t> ||
            std::is_same_v<T, float> ||
            std::is_same_v<T, double> ||
            std::is_same_v<T, std::string> >>
    struct is_value_retriever_implemented_simple : std::true_type {};
}

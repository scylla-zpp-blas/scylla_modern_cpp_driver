#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include "value_converters/statement_binder.hh"

namespace scmd {
class statement {
    CassStatement *_stmt;
    int _bind_idx = 0;

public:
    explicit statement(CassStatement *stmt);
    statement(const std::string &query, size_t arg_count);
    explicit statement(const std::string &query);

    template <typename... Types>
    statement &bind(Types... args) {
        (scmd_internal::bind_to_statement(_stmt, _bind_idx++, args), ...);
        return *this;
    }

    void reset(size_t arg_count);

    // We can't really copy this class
    statement(const statement &other) = delete;
    statement &operator=(const statement &other) = delete;

    statement(statement &&other) noexcept;
    statement &operator=(statement &&other) noexcept;

    [[nodiscard]] const CassStatement *get_statement() const;

    ~statement();
};
}  // namespace scmd

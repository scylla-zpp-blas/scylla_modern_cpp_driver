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
    statement(std::string &&query, size_t arg_count);
    explicit statement(std::string &&query);

    // We can't really copy this class
    statement(const statement &other) = delete;
    statement &operator=(const statement &other) = delete;

    statement(statement &&other) noexcept;
    statement &operator=(statement &&other) noexcept;

    ~statement();

    [[nodiscard]] const CassStatement *get_statement() const;

    [[nodiscard]] CassStatement *get_statement();

    template <typename... Types>
    statement &bind(Types... args) {
        (scmd_internal::bind_to_statement(_stmt, _bind_idx++, args), ...);
        return *this;
    }

    statement &set_timeout(uint64_t timeout);

    statement &reset(size_t arg_count);
};
}  // namespace scmd

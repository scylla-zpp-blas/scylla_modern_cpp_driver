#pragma once

#include <memory>
#include <stdexcept>
#include <string>

#include "cassandra.h"

namespace scmd {

class session;
class statement;

class prepared_query {
    const CassPrepared *_prepared;

public:
    explicit prepared_query(const CassPrepared *prepared);
    prepared_query(session &s, const std::string &query);

    // We can't really copy this class
    prepared_query(const prepared_query &other) = delete;
    prepared_query &operator=(const prepared_query &other) = delete;

    prepared_query(prepared_query &&other) noexcept;
    prepared_query &operator=(prepared_query &&other) noexcept;

    ~prepared_query();

    [[nodiscard]] statement get_statement() const;
};
}  // namespace scmd

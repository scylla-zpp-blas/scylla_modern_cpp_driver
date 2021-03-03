#pragma once
#include "cassandra.h"

#include "statement.hh"

namespace scmd {
class batch_query {
private:
    CassBatch *_batch;
public:
    explicit batch_query(CassBatchType type);

    // We can't really copy this class.
    batch_query(const statement &other) = delete;
    batch_query &operator=(const statement &other) = delete;

    batch_query(batch_query &&other) noexcept;
    batch_query &operator=(batch_query &&other) noexcept;

    ~batch_query();

    [[nodiscard]] const CassBatch *get_query() const;

    void set_keyspace(const std::string& keyspace);

    void set_consistency(CassConsistency consistency);

    void set_serial_consistency(CassConsistency consistency);

    void set_timestamp(int64_t timestamp);

    void set_timeout(uint64_t timeout);

    void add_statement(statement& statement);
};
}  // namespace scmd
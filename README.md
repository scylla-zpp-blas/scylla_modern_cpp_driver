# Scylla Modern C++ Driver
This project (Scylla Modern C++ Driver, later referred to as SCMD) aims to provide easy way to use Scylla from C++. DataStax cpp driver only exports C API, which isn't convenient to use in C++. It does use C++ internally, but unfortunately this internal API is not intended to be used by library users. SCMD is a small library that wraps this C API to provide classes, RAII, templates (instead of functions with type in name) etc.

## Building
Put this repo in a folder in your project (e.g. as a git submodule), and in your CMakeLists.txt add line:
```
add_subdirectory(path/to/driver EXCLUDE_FROM_ALL)
```
Then you need to link library to targets that use it:
```
target_link_libraries(your_library scylla_modern_cpp_driver)
```

That's it, now you can just `#include "scmd.hh"` and use library.

## Testing
Tests are built when SCMD is built as a standalone project, not a subdirectory. To use them, you need to have a working scylla running - there is a script `run_scylla.sh` in the repo to run scylla in docker. To run tests execute  `./build_directory/tests/tests -- scylla_ip`.

## Usage
```C++
//====== Connecting =======
// Constructing scmd::session objects makes new connection to Scylla.
// Pass IP and optionally port of Scylla instance.
// This API is most likely going to change
scmd::session session("172.19.0.2", "4092");

//====== Basic queries =======
// Simple query, no parameters
scmd::statement stmt("SELECT * FROM test");

// Query with parameters. You need to pass number of parameters to constructor
scmd::statement stmt2("SELECT * FROM test WHERE param1 = ? AND param2 = ?", 2);
// It's important to pass variables with right types to the bind method, otherwise query will fail.
stmt2.bind(var1, var2);

// Sync query. Throws scmd::exception in case of failure.
scmd::query_result result = session.execute(stmt);

//====== Async queries =======
scmd::future future = session.execute_async(stmt2);
// get_results() will return scmd::query_result or throw exception
scmd::query_result result2 = future.get_result();

// There is also a .wait() method if you don't need results, e.g. in INSERT query,
// or you just want to wait for query to end, and get the result later.
scmd::future future2 = session.execute_async("INSERT INTO test (param_1, param_2) VALUES (?, ?)", var1, var2);
future2.wait();

//You can check if query is finished
future2.is_ready();

//====== Shorter code =======
// There are some helper methods to reduce boilerplate code (creating scmd::statement manually)
session.execute("INSERT INTO test (param_1, param_2) VALUES (?, ?)", var1, var2);
auto res2 = session.execute("SELECT * FROM test");

//====== Prepared queries =======
// If you need to execute same query (possibly with different parametrers) multiple times,
// you may want to preprocess it to achieve better performance.
scmd::prepared_query prep = session.prepare("INSERT INTO test (param_1, param_2) VALUES (?, ?)");

// Now, when you want to execute this query
scmd::statement stmt = prep.get_statement();
stmt.bind(var_1, var_2);
session.execute(stmt);

// Or, alternatively, for shorter code
session.execute(prep.get_statement(), var_1, var_2);


//====== Batch queries =======
scmd::batch_query batch(CASS_BATCH_TYPE_UNLOGGED);
for(int i = 0; i < 1000; i++) {
	scmd::statement stmt = get_some_statement();
	batch.add_statement(stmt);
}
session.execute(batch);

//====== Reading results =======
// This API is most likely going to change
scmd::query_result result = session.execute("SELECT * FROM test");

// Self-explanatory: .row_count() returns how many rows were returned
fmt::print("Returned rows: %d\n", result.row_count());

// You can check what column is in which place.
fmt::print("Name of column 0: %s\n", result.get_column_name(0));

// .next_row() moves internal iterator to new row. Returns false if there are no more rows
while(result.next_row()) {
	// Process row here.

	// .get_column return value from given column.
	// Throws scmd::exception in case anything goes wrong.
	// You can call .get_column<type>(arg) with column name or column index.
	int64_t val = result.get_column<int64_t>("param_1");
	std::string val2 = result.get_column<std::string>(2);
	
	// If .get_column is not implemented for some type, you can get raw value.
	CassValue *val3 = result.get_column_raw(3);

	// You can call this method with name/index as well
	bool is_param_1_null = result.is_column_null("param_1");
}
```

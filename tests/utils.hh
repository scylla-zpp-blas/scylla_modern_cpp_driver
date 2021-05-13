#pragma once

#include <cstdint>

#define ARGS int8_t(115), int16_t(3456), int32_t(104755), int64_t(123341233124), float(30.75), double(32342.23), std::string("sadasdasda")
#define TYPES int64_t, int8_t, int16_t, int32_t, int64_t, float, double, std::string
#define ARGS_TUPLE(id) std::make_tuple(id, ARGS)
#define RESULT_TUPLE(r) std::make_tuple(r.get_column<int64_t>("key"), \
                                        r.get_column<int8_t>("a"),    \
                                        r.get_column<int16_t>("b"), \
                                        r.get_column<int32_t>("c"), \
                                        r.get_column<int64_t>("d"), \
                                        r.get_column<float>("e"), \
                                        r.get_column<double>("f"), \
                                        r.get_column<std::string>("g"))

int64_t generate_id();
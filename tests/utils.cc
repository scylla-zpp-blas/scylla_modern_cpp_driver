#include "utils.hh"


int64_t generate_id() {
    static int64_t id = 0;
    return id++;
}
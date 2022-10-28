#pragma once
#include <stddef.h>

struct TypeDesc_Test {
    int build_number;
    size_t size;
    size_t off_boolean;
    size_t off_conststr;
    size_t off_field_a;
    size_t off_flags;
    size_t off_pointer;
    size_t off_ppp;
    size_t off_sss;
};
static const size_t neg1 = (size_t)-1;
static const struct TypeDesc_Test __gtd_Test[2] = {
    { 9200, 48, 8, 40, 0, 4, 16, 24, neg1 },
    { 9600, 48, 8, 40, 0, 4, 16, 24, 32 },
};

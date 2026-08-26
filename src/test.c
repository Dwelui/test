#include <dwelui/test.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>

extern const Test __start_tests[];
extern const Test __stop_tests[];

int               main() {
    for (const Test *test = __start_tests; test < __stop_tests; ++test) {
        test->fn();
    }

    return 0;
}

void dwelui__test_assert_impl(bool condition, const char *expression, const char *file, u_int8_t line) {
    if (false == condition) {
        fprintf(stderr, "%s:%d: assertion failed: %s\n", file, line, expression);
    }
}

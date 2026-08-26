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

void dwelui__test_assert_fail(const char *message, const char *file, u_int32_t line,
                              const char *function) {
    fprintf(stderr, "%s:%u: %s: assertion failed: %s\n", file, line, function, message);
}

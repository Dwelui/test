#include <dwelui/test.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>

typedef enum {
    TEST_OK,
    TEST_FAILED,
    TEST_SKIPPED,
} TEST_STATUS;

extern Test __start_tests[];
extern Test __stop_tests[];

int               main() {
    for (Test *test = __start_tests; test < __stop_tests; ++test) {
        test->status = TEST_SKIPPED;

        if (test->fn() == 0) {
            test->status = TEST_OK;
        } else {
            test->status = TEST_FAILED;
        }
    }

    return 0;
}

void dwelui__test_assert_fail(const char *message, const char *file, u_int32_t line,
                              const char *function) {
    fprintf(stderr, "%s:%u: %s: assertion failed: %s\n", file, line, function, message);

    // TODO: find TEST by name and file and set it as failed. Might want to set message aswell instead of printing.
}

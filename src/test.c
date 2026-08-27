#include <dwelui/test.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

typedef enum {
    TEST_STATUS_PASSED,
    TEST_STATUS_FAILED,
    TEST_STATUS_PENDING,
} TEST_STATUS;

typedef struct {
    Test  *tests;
    size_t count;
    size_t capacity;
} TestList;

TestList testList = {.count = 0, .capacity = 1024};

int      main() {
    for (size_t i = 0; i < testList.count; i++) {
        Test *test = &testList.tests[i];

        if (0 == test->fn()) {
            test->status = TEST_STATUS_PASSED;
        } else {
            test->status = TEST_STATUS_FAILED;
        }
    }

    return 0;
}

void dwelui__test_register(const char *name, TestFn fn) {
    if (testList.count == 0) {
        testList.tests = malloc(sizeof(Test) * testList.capacity);
    }

    testList.tests[testList.count++] = (Test){name, fn, .status = TEST_STATUS_PENDING};
}

void dwelui__test_assert_fail(const char *message, const char *file, u_int32_t line,
                              const char *function) {
    fprintf(stderr, "%s:%u: %s: assertion failed: %s\n", file, line, function, message);

    // TODO: find TEST by name and file and set it as failed. Might want to set message aswell instead of printing.
}

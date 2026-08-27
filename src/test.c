#include <dwelui/test.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

// TODO: Rename enum values to TEST_STATUS_*
typedef enum {
    TEST_OK,
    TEST_FAILED,
    TEST_PENDING,
    TEST_SKIPPED,
} TEST_STATUS;

typedef struct {
    Test     *tests;
    u_int64_t count;
    u_int64_t capacity;
} TestList;

TestList testList = {.count = 0, .capacity = 1024};

int main() {

    for (Test *test = testList.tests; test < &testList.tests[testList.count]; ++test) {
        test->fn();
    }

    return 0;
}

void dwelui__test_register(const char *name, TestFn fn) {
    if (testList.count == 0) {
        testList.tests = malloc(sizeof(Test) * testList.capacity);
    }

    testList.tests[testList.count++] = (Test){name, fn, .status = TEST_PENDING};
}

void dwelui__test_assert_fail(const char *message, const char *file, u_int32_t line,
                              const char *function) {
    fprintf(stderr, "%s:%u: %s: assertion failed: %s\n", file, line, function, message);

    // TODO: find TEST by name and file and set it as failed. Might want to set message aswell instead of printing.
}

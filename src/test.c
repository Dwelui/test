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
        Test           *test       = &testList.tests[i];
        TestFailResult *failResult = test->fn();

        if (nullptr == failResult) {
            test->status = TEST_STATUS_PASSED;
        } else {
            test->status = TEST_STATUS_FAILED;

            test->failResult = failResult;
        }
    }

    for (size_t i = 0; i < testList.count; i++) {
        Test *test = &testList.tests[i];

        switch (test->status) {
            case TEST_STATUS_FAILED:
                TestFailResult *failResult = test->failResult;
                fprintf(stderr, "%s:%u: %s: assertion failed: %s\n", test->file, failResult->line,
                        test->name, failResult->message);

                free(failResult);
                break;
            case TEST_STATUS_PASSED:
                fprintf(stdout, "%s:%u: %s: assertion passed \n", test->file, test->line,
                        test->name);
                break;
        }
    }

    free(testList.tests);

    return 0;
}

void dwelui__test_register(const char *name, const char *file, u_int32_t line, TestFn fn) {
    if (testList.count == 0) {
        testList.tests = malloc(sizeof(Test) * testList.capacity);
        if (!testList.tests) return;
    }

    testList.tests[testList.count++] =
        (Test){name, file, line, fn, .status = TEST_STATUS_PENDING, .failResult = nullptr};
}

TestFailResult *dwelui__test_fail(const char *message, u_int32_t line) {
    TestFailResult *result = malloc(sizeof(*result));
    if (!result) return nullptr;
    *result = (TestFailResult){message, line};

    return result;
}

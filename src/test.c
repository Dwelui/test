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

typedef struct {
    TestResult *results;
    size_t      count;
    size_t      capacity;
} TestResultList;

TestList       testList       = {.count = 0, .capacity = 1024};
TestResultList testResultList = {.count = 0, .capacity = 1024};

int            main() {
    for (size_t i = 0; i < testList.count; i++) {
        Test       *test   = &testList.tests[i];
        TestResult *result = test->fn();

        result->test = test;
    }

    for (size_t i = 0; i < testResultList.count; i++) {
        TestResult *result = &testResultList.results[i];
        Test       *test   = result->test;

        switch (result->status) {
            case TEST_STATUS_FAILED:
                fprintf(stderr, "%s:%u: %s: assertion failed: %s\n", test->file, result->line,
                        test->name, result->message);

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

    testList.tests[testList.count++] = (Test){name, file, line, fn};
}

TestResult *test_result_create() {
    if (testResultList.count == 0) {
        testResultList.results = malloc(sizeof(testResultList) * testResultList.capacity);
        if (!testResultList.results) return nullptr;
    }

    TestResult *result = &testResultList.results[testResultList.count++];
    *result =
        (TestResult){.test = nullptr, .status = TEST_STATUS_PENDING, .message = nullptr, .line = 0};

    return result;
}

TestResult *dwelui__test_fail(const char *message, u_int32_t line) {
    TestResult *result = test_result_create();
    result->status = TEST_STATUS_FAILED;
    result->message = message;
    result->line = line;

    return result;
}

TestResult *dwelui__test_pass() {
    TestResult *result = test_result_create();
    result->status = TEST_STATUS_PASSED;

    return result;
}

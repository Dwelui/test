#include <dwelui/test.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    TEST_RESULT_STATUS_PASSED,
    TEST_RESULT_STATUS_FAILED,
    TEST_RESULT_STATUS_PENDING,
} TEST_RESULT_STATUS;
const char *test_result_status_to_cstring(TEST_RESULT_STATUS status);

typedef struct {
    Test  *items;
    size_t count;
    size_t capacity;
} TestList;

typedef struct {
    TestResult *items;
    size_t      count;
    size_t      capacity;
} TestResultList;

TestList       testList       = {.count = 0, .capacity = 1024};
TestResultList testResultList = {.count = 0, .capacity = 1024};

void           print_file(const char *file);

int            main() {
    for (size_t i = 0; i < testList.count; i++) {
        Test       *test   = &testList.items[i];
        TestResult *result = test->fn();

        result->test = test;
    }

    const char *file       = nullptr;
    const char *treePrefix = "  ├";
    for (size_t i = 0; i < testResultList.count; i++) {
        TestResult *result = &testResultList.items[i];
        TestResult *nextResult =
            i + 1 == testResultList.count ? nullptr : &testResultList.items[i + 1];
        Test *test     = result->test;
        Test *nextTest = nextResult == nullptr ? nullptr : nextResult->test;

        treePrefix = "  ├";

        if (file == nullptr || strcmp(file, test->file) != 0) {
            print_file(test->file);

            file = test->file;
        }

        if (nextTest == nullptr || strcmp(test->file, nextTest->file) != 0) {
            treePrefix = "  └";
        }

        printf("%s%s assertion %s\n", treePrefix, test->name,
               test_result_status_to_cstring(result->status));
    }

    free(testList.items);

    return 0;
}

void dwelui__test_register(const char *name, const char *file, uint32_t line, TestFn fn) {
    if (testList.count == 0) {
        testList.items = malloc(sizeof(Test) * testList.capacity);
        if (!testList.items) return;
    }

    // The "file" could be reusable array. To save space and checking for each test in a same file.
    testList.items[testList.count++] = (Test){name, file, line, fn};
}

TestResult *test_result_create() {
    if (testResultList.count == 0) {
        testResultList.items = malloc(sizeof(testResultList) * testResultList.capacity);
        if (!testResultList.items) return nullptr;
    }

    TestResult *result = &testResultList.items[testResultList.count++];
    *result            = (TestResult){.test         = nullptr,
                                      .status       = TEST_RESULT_STATUS_PENDING,
                                      .fail_message = nullptr,
                                      .fail_line    = 0};

    return result;
}

TestResult *dwelui__test_fail(const char *message, uint32_t line) {
    TestResult *result   = test_result_create();
    result->status       = TEST_RESULT_STATUS_FAILED;
    result->fail_message = message;
    result->fail_line    = line;

    return result;
}

TestResult *dwelui__test_pass() {
    TestResult *result = test_result_create();
    result->status     = TEST_RESULT_STATUS_PASSED;

    return result;
}

void print_file(const char *file) {
    const char *startPtr = strstr(file, "/") + 1;
    const char *endPtr   = strstr(file, ".c");
    size_t      length   = endPtr - startPtr;

    char       *formatted = malloc(length + 1);
    memcpy(formatted, startPtr, length);
    formatted[length] = '\0';

    printf("%s\n", formatted);

    free(formatted);
}

const char *test_result_status_to_cstring(TEST_RESULT_STATUS status) {
    switch (status) {
        case TEST_RESULT_STATUS_PASSED:
            return "passed";
        case TEST_RESULT_STATUS_FAILED:
            return "failed";
        case TEST_RESULT_STATUS_PENDING:
            return "pending";
        default:
            abort();
    };
}

#include <dwelui/test.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define C_RED   "\x1b[31m"
#define C_GREEN "\x1b[32m"
#define C_RESET "\x1b[0m"

typedef enum {
    TEST_STATUS_PASSED,
    TEST_STATUS_FAILED,
    TEST_STATUS_PENDING,
} TEST_STATUS;
const char *test_status_to_cstring(TEST_STATUS status);

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

void print_test_failed_information(bool nextTestExists, const char *file, TestResult *result);
void print_run_header(size_t discoveredTests);
void print_file(const char *file);
void print_test_status(const char *treePrefix, Test *test);

int  main() {
    print_run_header(testList.count);

    for (size_t i = 0; i < testList.count; i++) {
        Test       *test   = &testList.items[i];
        TestResult *result = test->fn();

        test->status = result->status;
        result->test = test;
    }

    // naive implementation of output formatting, does not support multiple levels.
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

        print_test_status(treePrefix, test);

        if (result->status == TEST_STATUS_FAILED) {
            print_test_failed_information(nextTest != nullptr, test->file, result);
        }
    }

    size_t testCount = testList.count, testPassedCount = 0, testFailedCount = 0;
    for (size_t i = 0; i < testList.count; i++) {
        Test *test = &testList.items[i];

        switch (test->status) {
            case TEST_STATUS_PASSED:
                testPassedCount++;
                break;
            case TEST_STATUS_FAILED:
                testFailedCount++;
                break;
        }
    }

    printf("\nTotal: %lu | " C_GREEN "Passed: %lu" C_RESET " | " C_RED "Failed: %lu" C_RESET " \n",
           testCount, testPassedCount, testFailedCount);

    free(testList.items);
    free(testResultList.items);

    return 0;
}

void print_run_header(size_t discoveredTests) {
    printf("libtest\n");
    printf("Author: Dwelui\n");
    printf("Status: starting test runtime\n");
    printf("Discovered tests: %lu\n", discoveredTests);
    printf("----------------------------------------\n");
}

void dwelui__test_register(const char *name, const char *file, uint32_t line, TestFn fn) {
    if (testList.count == 0) {
        testList.items = malloc(sizeof(Test) * testList.capacity);
        if (!testList.items) return;
    }

    // The "file" could be reusable array. To save space and checking for each test in a same file.
    testList.items[testList.count++] = (Test){name, file, line, fn, .status = TEST_STATUS_PENDING};
}

TestResult *test_result_create() {
    if (testResultList.count == 0) {
        testResultList.items = malloc(sizeof(testResultList) * testResultList.capacity);
        if (!testResultList.items) return nullptr;
    }

    TestResult *result = &testResultList.items[testResultList.count++];
    *result            = (TestResult){
        .test = nullptr, .status = TEST_STATUS_PENDING, .fail_message = nullptr, .fail_line = 0};

    return result;
}

TestResult *dwelui__test_fail(const char *message, uint32_t line) {
    TestResult *result   = test_result_create();
    result->status       = TEST_STATUS_FAILED;
    result->fail_message = message;
    result->fail_line    = line;

    return result;
}

TestResult *dwelui__test_pass() {
    TestResult *result = test_result_create();
    result->status     = TEST_STATUS_PASSED;

    return result;
}

void print_test_status(const char *treePrefix, Test *test) {
    switch (test->status) {
        case TEST_STATUS_PASSED:
            printf(C_GREEN "%s%s %s" C_RESET "\n", treePrefix, test->name,
                   test_status_to_cstring(test->status));
            return;
        case TEST_STATUS_FAILED:
            printf(C_RED "%s%s %s" C_RESET "\n", treePrefix, test->name,
                   test_status_to_cstring(test->status));
            return;
        default:
            printf("%s%s %s\n", treePrefix, test->name, test_status_to_cstring(test->status));
    };
}

void print_test_failed_information(bool nextTestExists, const char *file, TestResult *result) {

    const char *treeFailedPrefix = "  │";
    if (false == nextTestExists) {
        treeFailedPrefix = "   ";
    }

    printf(C_RED "%s  └%s:%u :: " C_RED "\"%s\"" C_RESET "\n", treeFailedPrefix, file,
           result->fail_line, result->fail_message);
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

const char *test_status_to_cstring(TEST_STATUS status) {
    switch (status) {
        case TEST_STATUS_PASSED:
            return "passed";
        case TEST_STATUS_FAILED:
            return "failed";
        case TEST_STATUS_PENDING:
            return "pending";
        default:
            abort();
    };
}

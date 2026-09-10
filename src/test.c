#include <assert.h>
#include <dwelui/test.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define C_RED   "\x1b[31m"
#define C_GREEN "\x1b[32m"
#define C_BLUE  "\x1b[34m"
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

typedef struct {
    TestDataProvider *items;
    size_t            count;
    size_t            capacity;
} TestDataProviderList;

TestList             testList             = {.count = 0, .capacity = 1024};
TestResultList       testResultList       = {.count = 0, .capacity = 1024};
TestDataProviderList testDataProviderList = {.count = 0, .capacity = 1024};

void print_test_failed_information(bool nextTestExists, const char *file, TestResult *result);
void print_run_header(size_t discoveredTests, size_t discoveredDataProviders);
void print_file(const char *file);
void print_test_information(const char *treePrefix, Test *test, TestResult *result);
void print_totals_information();
void run_test(Test *test, TestData *data);

int  test() {
    print_run_header(testList.count, testDataProviderList.count);

    for (size_t i = 0; i < testList.count; i++) {
        Test             *test    = &testList.items[i];
        TestOptions       options = test->options;

        TestDataProvider *dataProvider = options.dataProvider;
        if (nullptr != dataProvider) {
            for (size_t y = 0; y < dataProvider->count; y++) {
                TestData *data = &dataProvider->items[y];

                run_test(test, data);
            }

            continue;
        }

        run_test(test, nullptr);
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

        print_test_information(treePrefix, test, result);

        if (result->status == TEST_STATUS_FAILED) {
            print_test_failed_information(nextTest != nullptr, test->file, result);
        }
    }

    print_totals_information();

    free(testList.items);
    free(testResultList.items);

    for (size_t i = 0; i < testDataProviderList.count; i++) {
        TestDataProvider *dataProvider = &testDataProviderList.items[i];
        free(dataProvider->items);
    }
    free(testDataProviderList.items);

    return 0;
}

void run_test(Test *test, TestData *data) {
    TestResult *result = test->fn(nullptr != data ? data->data : nullptr);
    result->test       = test;
    result->data       = data;

    if (test->status != TEST_STATUS_FAILED) {
        test->status = result->status;
    }
}

void dwelui__test_register(const char *name, const char *file, uint32_t line, TestFn fn) {
    if (testList.count == 0) {
        testList.items = malloc(sizeof(Test) * testList.capacity);
        assert(nullptr != testList.items);
    }

    assert(testList.count < testList.capacity);

    testList.items[testList.count++] =
        (Test){name, file, line, fn, .status = TEST_STATUS_PENDING, .options = {0}};
}

void dwelui__test_options_register(TestFn testFn, TestOptions options) {
    for (size_t i = 0; i < testList.count; i++) {
        Test *test = &testList.items[i];

        if (test->fn == testFn) {
            test->options = options;
        }
    }
}

TestResult *test_result_create() {
    if (testResultList.count == 0) {
        testResultList.items = malloc(sizeof(TestResult) * testResultList.capacity);
        assert(nullptr != testResultList.items);
    }

    assert(testResultList.count < testResultList.capacity);

    TestResult *result = &testResultList.items[testResultList.count++];
    *result            = (TestResult){.test        = nullptr,
                                      .status      = TEST_STATUS_PENDING,
                                      .failMessage = nullptr,
                                      .failLine    = 0,
                                      .data        = nullptr};

    return result;
}

void dwelui__test_data_provider_register(const char *name, TestDataProviderFn fn) {
    if (testDataProviderList.count == 0) {
        testDataProviderList.items =
            malloc(sizeof(TestDataProvider) * testDataProviderList.capacity);
        assert(nullptr != testDataProviderList.items);
    }

    assert(testDataProviderList.count < testDataProviderList.capacity);

    TestDataProvider *dataProvider = dwelui__test_data_provider_find(name);
    if (nullptr != dataProvider) {
        return;
    }

    dataProvider        = &testDataProviderList.items[testDataProviderList.count];
    *dataProvider       = (TestDataProvider){name, .items = nullptr, .count = 0, .capacity = 1024};
    dataProvider->items = malloc(sizeof(TestData) * dataProvider->capacity);
    assert(nullptr != dataProvider->items);

    testDataProviderList.items[testDataProviderList.count++] = *dataProvider;

    fn();
}

TestDataProvider *dwelui__test_data_provider_find(const char *name) {
    for (size_t i = 0; i < testDataProviderList.count; i++) {
        TestDataProvider *dataProvider = &testDataProviderList.items[i];
        if (strcmp(dataProvider->name, name) == 0) {
            return dataProvider;
        }
    }

    return nullptr;
}

TestResult *dwelui__test_fail(const char *message, uint32_t line) {
    TestResult *result  = test_result_create();
    result->status      = TEST_STATUS_FAILED;
    result->failMessage = message;
    result->failLine    = line;

    return result;
}

TestResult *dwelui__test_pass() {
    TestResult *result = test_result_create();
    result->status     = TEST_STATUS_PASSED;

    return result;
}

void dwelui__test_data_add(TestDataProvider *dataProvider, const char *name, const void *items) {
    assert(dataProvider->count < dataProvider->capacity);
    dataProvider->items[dataProvider->count++] = (TestData){name, items};
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
            assert(false);
    };
}

void print_run_header(size_t discoveredTests, size_t discoveredDataProviders) {
    printf("test\n");
    printf("Author: Dwelui\n");
    printf("Status: running tests\n");
    printf("Discovered tests: %lu\n", discoveredTests);
    printf("Discovered data providers: %lu\n", discoveredDataProviders);
    printf("----------------------------------------\n");
}

void print_test_information(const char *treePrefix, Test *test, TestResult *result) {
    switch (test->status) {
        case TEST_STATUS_PASSED:
            printf("%s%s " C_GREEN "%s" C_RESET, treePrefix, test->name,
                   test_status_to_cstring(test->status));
            break;
        case TEST_STATUS_FAILED:
            printf("%s%s " C_RED "%s" C_RESET, treePrefix, test->name,
                   test_status_to_cstring(test->status));
            break;
        default:
            printf("%s%s %s\n", treePrefix, test->name, test_status_to_cstring(test->status));
            break;
    };

    TestData *data = result->data;
    if (nullptr != data) {
        printf(C_BLUE " with \"%s\"" C_RESET, data->name);
    }

    printf("\n");
}

void print_test_failed_information(bool nextTestExists, const char *file, TestResult *result) {

    const char *treeFailedPrefix = "  │";
    if (false == nextTestExists) {
        treeFailedPrefix = "   ";
    }

    printf("%s " C_RED " └%s:%u error " C_RED "\"%s\"" C_RESET "\n", treeFailedPrefix, file,
           result->failLine, result->failMessage);
}

void print_file(const char *file) {
    const char *startPtr = strstr(file, "/") + 1;
    const char *endPtr   = strstr(file, ".test.c");
    size_t      length   = endPtr - startPtr;

    char       *formatted = malloc(length + 1);
    memcpy(formatted, startPtr, length);
    formatted[length] = '\0';

    printf("%s\n", formatted);

    free(formatted);
}

void print_totals_information() {
    printf("----------------------------------------\n");

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

    size_t testResultCount = testResultList.count, testResultPassedCount = 0,
           testResultFailedCount = 0;
    for (size_t i = 0; i < testResultList.count; i++) {
        TestResult *testResult = &testResultList.items[i];

        switch (testResult->status) {
            case TEST_STATUS_PASSED:
                testResultPassedCount++;
                break;
            case TEST_STATUS_FAILED:
                testResultFailedCount++;
                break;
        }
    }

    printf("Total tests:   %5lu | " C_GREEN "Passed: %5lu" C_RESET " | " C_RED "Failed: %5lu" C_RESET
           " \n",
           testCount, testPassedCount, testFailedCount);

    printf("Total results: %5lu | " C_GREEN "Passed: %5lu" C_RESET " | " C_RED "Failed: %5lu" C_RESET
           " \n",
           testResultCount, testResultPassedCount, testResultFailedCount);
}

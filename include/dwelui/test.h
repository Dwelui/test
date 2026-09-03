#ifndef DWELUI_TEST_H
#define DWELUI_TEST_H

#include <stddef.h>
#include <stdint.h>

typedef struct Test             Test;
typedef struct TestResult       TestResult;
typedef struct TestData         TestData;
typedef struct TestDataProvider TestDataProvider;
typedef struct TestOptions      TestOptions;

typedef TestResult *(*TestFn)(TestData *data);

struct TestOptions {
    TestDataProvider *dataProvider;
};

struct TestData {
    const char *name;
    const void *items;
};

struct TestDataProvider {
    const char *name;
    TestData   *items;
    size_t      count;
    size_t      capacity;
};

struct Test {
    const char *name;
    const char *file;
    uint32_t    line;
    TestFn      fn;
    uint8_t     status;
    TestOptions options;
};

struct TestResult {
    Test       *test;
    uint8_t     status;
    const char *failMessage;
    uint32_t    failLine;
    TestData   *data;
};

extern void              dwelui__test_register(const char *, const char *, uint32_t, TestFn);
extern TestResult       *dwelui__test_fail(const char *, uint32_t);
extern TestResult       *dwelui__test_pass();

extern void              dwelui__test_data_provider_register(TestDataProvider *);
extern TestDataProvider *dwelui__test_data_provider_initialize(const char *);

extern void              dwelui__test_options_register(TestFn, TestOptions);
extern void              dwelui__test_data_add(TestDataProvider *, const char *, const void *);

#define test_assert(expr)                                                                          \
    if (false == (expr)) return dwelui__test_fail(#expr, __LINE__)

#define TEST(name, ...)                                                                            \
    static TestResult *name##_test(TestData *data);                                                \
                                                                                                   \
    static void        register_##name##_test(void) __attribute__((constructor));                  \
    static void        register_##name##_test(void) {                                              \
        dwelui__test_register(#name, __FILE__, __LINE__, name##_test);                             \
    }                                                                                              \
                                                                                                   \
    static TestResult *name##_test(TestData *data) {                                               \
        (void)data;                                                                                \
        __VA_ARGS__                                                                                \
        return dwelui__test_pass();                                                                \
    }

#define TEST_OPTIONS(testName, ...)                                                                \
    static void register_##testName##_options(void) __attribute__((constructor));                  \
    static void register_##testName##_options(void) {                                              \
        dwelui__test_options_register(testName##_test, (TestOptions){__VA_ARGS__});                \
    }

// TODO: register at init time, each call for DATA_PROVIDER runs the data generation each time.
// Generate data once and reuse it.
#define DATA_PROVIDER(name, ...)                                                                   \
    static TestDataProvider *name##_data_provider();                                                               \
                                                                                                   \
    static TestDataProvider *name##_data_provider() {                                                              \
        TestDataProvider *dataProvider = dwelui__test_data_provider_initialize(#name);             \
        dwelui__test_data_provider_register(dataProvider);                                         \
        __VA_ARGS__                                                                                \
        return dataProvider;                                                                       \
    }

#define test_data_add(name, items) dwelui__test_data_add(dataProvider, name, &items)

#endif // DWELUI_TEST_H

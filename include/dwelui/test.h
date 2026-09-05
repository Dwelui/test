#ifndef DWELUI_TEST_H
#define DWELUI_TEST_H

#include <stddef.h>
#include <stdint.h>

typedef struct Test             Test;
typedef struct TestResult       TestResult;
typedef struct TestData         TestData;
typedef struct TestDataProvider TestDataProvider;
typedef struct TestOptions      TestOptions;

typedef TestResult *(*TestFn)(const void *);
typedef void (*TestDataProviderFn)(void);

struct TestOptions {
    TestDataProvider *dataProvider;
};

struct TestData {
    const char *name;
    const void *data;
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

int               test();

void              dwelui__test_register(const char *, const char *, uint32_t, TestFn);
TestResult       *dwelui__test_fail(const char *, uint32_t);
TestResult       *dwelui__test_pass();

void              dwelui__test_data_provider_register(const char *, TestDataProviderFn);
TestDataProvider *dwelui__test_data_provider_find(const char *);

void              dwelui__test_options_register(TestFn, TestOptions);
void              dwelui__test_data_add(TestDataProvider *, const char *, const void *);

#define test_assert(expr)                                                                          \
    if (false == (expr)) return dwelui__test_fail(#expr, __LINE__)

#define TEST(name, ...)                                                                            \
    static TestResult *name##_test(const void *data);                                              \
                                                                                                   \
    static void        register_##name##_test(void) __attribute__((constructor));                  \
    static void        register_##name##_test(void) {                                              \
        dwelui__test_register(#name, __FILE__, __LINE__, name##_test);                             \
    }                                                                                              \
                                                                                                   \
    static TestResult *name##_test(const void *data) {                                             \
        (void)data;                                                                                \
        __VA_ARGS__                                                                                \
        return dwelui__test_pass();                                                                \
    }

#define TEST_OPTIONS(testName, ...)                                                                \
    static void register_##testName##_options(void) __attribute__((constructor));                  \
    static void register_##testName##_options(void) {                                              \
        dwelui__test_options_register(testName##_test, (TestOptions){__VA_ARGS__});                \
    }

#define DATA_PROVIDER(name, ...)                                                                   \
    void        name##_data_provider();                                                            \
                                                                                                   \
    static void register_##name##_data_provider(void) __attribute__((constructor));                \
    static void register_##name##_data_provider(void) {                                            \
        dwelui__test_data_provider_register(#name, name##_data_provider);                          \
    }                                                                                              \
                                                                                                   \
    static TestDataProvider *name() {                                                              \
        return dwelui__test_data_provider_find(#name);                                             \
    }                                                                                              \
                                                                                                   \
    void name##_data_provider() {                                                                  \
        TestDataProvider *dataProvider = dwelui__test_data_provider_find(#name);                   \
        __VA_ARGS__                                                                                \
    }

#define test_data_add(name, items) dwelui__test_data_add(dataProvider, name, &items)
#define test_data_get(type)        ((const type *)(data))

#endif // DWELUI_TEST_H

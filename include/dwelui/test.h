#ifndef DWELUI_TEST_H
#define DWELUI_TEST_H

#include <stdint.h>

typedef struct Test       Test;
typedef struct TestResult TestResult;

typedef TestResult *(*TestFn)(void);

struct Test {
    const char *name;
    const char *file;
    uint32_t    line;
    TestFn      fn;
};

struct TestResult {
    Test       *test;
    uint8_t     status;
    const char *fail_message;
    uint32_t    fail_line;
};

#define test_assert(expr)                                                                          \
    do {                                                                                           \
        extern TestResult *dwelui__test_fail(const char *, uint32_t);                              \
                                                                                                   \
        if (false == (expr)) {                                                                     \
            return dwelui__test_fail(#expr, __LINE__);                                             \
        }                                                                                          \
    } while (0)

#define TEST(fn_name, body)                                                                        \
    static TestResult *fn_name(void);                                                              \
                                                                                                   \
    extern void        dwelui__test_register(const char *, const char *, uint32_t, TestFn);        \
    extern TestResult *dwelui__test_pass();                                                        \
                                                                                                   \
    static void        register_##fn_name(void) __attribute__((constructor));                      \
    static void        register_##fn_name(void) {                                                  \
        dwelui__test_register(#fn_name, __FILE__, __LINE__, fn_name);                              \
    }                                                                                              \
                                                                                                   \
    static TestResult *fn_name(void) {                                                             \
        body return dwelui__test_pass();                                                           \
    }

#endif // DWELUI_TEST_H

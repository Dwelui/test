#ifndef DWELUI_TEST_H
#define DWELUI_TEST_H

#include <stdint.h>
#include <sys/types.h>

typedef struct {
    const char *message;
    u_int32_t   line;
} TestResult;

typedef TestResult *(*TestFn)(void);

typedef struct {
    const char *name;
    const char *file;
    u_int32_t   line;
    TestFn      fn;
    u_int8_t    status;
    TestResult *result;
} Test;

#define test_assert(expr)                                                                          \
    do {                                                                                           \
        extern TestResult *dwelui__test_fail(const char *, u_int32_t);                             \
        if (false == (expr)) {                                                                     \
            return dwelui__test_fail(#expr, __LINE__);                                             \
        }                                                                                          \
    } while (0)

#define TEST(fn_name, body)                                                                        \
    static TestResult *fn_name(void);                                                              \
                                                                                                   \
    extern void        dwelui__test_register(const char *, const char *, u_int32_t, TestFn);       \
    static void        register_##fn_name(void) __attribute__((constructor));                      \
    static void        register_##fn_name(void) {                                                  \
        dwelui__test_register(#fn_name, __FILE__, __LINE__, fn_name);                              \
    }                                                                                              \
                                                                                                   \
    static TestResult *fn_name(void) {                                                             \
        body return nullptr;                                                                       \
    }

#endif // DWELUI_TEST_H

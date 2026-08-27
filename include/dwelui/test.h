#ifndef DWELUI_TEST_H
#define DWELUI_TEST_H

#include <stdint.h>
#include <sys/types.h>

typedef struct {
    const char *function;
    const char *message;
    const char *file;
    u_int32_t   line;
} TestFailResult;

typedef TestFailResult *(*TestFn)(void);

// INFO: Needed for macro registration using sections. Remove from header later when registration mechanism changes.
typedef struct {
    const char     *name;
    TestFn          fn;
    u_int8_t        status;
    TestFailResult *failResult;
} Test;

#define test_assert(expr)                                                                          \
    do {                                                                                           \
        extern TestFailResult *dwelui__test_fail(const char *, const char *, u_int32_t,            \
                                                 const char *);                                    \
        if (false == (expr)) {                                                                     \
            return dwelui__test_fail(#expr, __FILE__, __LINE__, __func__);                         \
        }                                                                                          \
    } while (0)

#define TEST(fn_name, body)                                                                        \
    static TestFailResult *fn_name(void);                                                          \
                                                                                                   \
    extern void            dwelui__test_register(const char *, TestFn);                            \
    static void            register_##fn_name(void) __attribute__((constructor));                  \
    static void            register_##fn_name(void) {                                              \
        dwelui__test_register(#fn_name, fn_name);                                                  \
    }                                                                                              \
                                                                                                   \
    static TestFailResult *fn_name(void) {                                                         \
        body return nullptr;                                                                       \
    }

#endif // DWELUI_TEST_H

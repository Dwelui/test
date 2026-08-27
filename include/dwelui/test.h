#ifndef DWELUI_TEST_H
#define DWELUI_TEST_H

#include <stdint.h>
#include <sys/types.h>

typedef struct {
    const char *message;
    u_int32_t   line;
} TestFailResult;

typedef TestFailResult *(*TestFn)(void);

// The structure in the future could be:
// many to many relationship between `TestDataProvider` and `Test`
// many to one `TestResult` with `Test`. `TestResult` could have optional pointer to `TestDataProvider`
//
// Proposed changes for such structure:
// Move pointer owning to `TestResult`. Each `TestResult` could store "status" and other metadata like:
// test duration
typedef struct {
    const char     *name;
    const char     *file;
    u_int32_t       line;
    TestFn          fn;
    u_int8_t        status;
    TestFailResult *failResult;
} Test;

#define test_assert(expr)                                                                          \
    do {                                                                                           \
        extern TestFailResult *dwelui__test_fail(const char *, u_int32_t);                         \
        if (false == (expr)) {                                                                     \
            return dwelui__test_fail(#expr, __LINE__);                                             \
        }                                                                                          \
    } while (0)

#define TEST(fn_name, body)                                                                        \
    static TestFailResult *fn_name(void);                                                          \
                                                                                                   \
    extern void            dwelui__test_register(const char *, const char *, u_int32_t, TestFn);   \
    static void            register_##fn_name(void) __attribute__((constructor));                  \
    static void            register_##fn_name(void) {                                              \
        dwelui__test_register(#fn_name, __FILE__, __LINE__, fn_name);                              \
    }                                                                                              \
                                                                                                   \
    static TestFailResult *fn_name(void) {                                                         \
        body return nullptr;                                                                       \
    }

#endif // DWELUI_TEST_H

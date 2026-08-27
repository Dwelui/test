#ifndef DWELUI_TEST_H
#define DWELUI_TEST_H

#include <stdint.h>
#include <sys/types.h>

typedef int (*TestFn)(void);

// INFO: Needed for macro registration using sections. Remove from header later when registration mechanism changes.
typedef struct {
    const char *name;
    TestFn      fn;
    u_int8_t    status;
} Test;

#define test_assert(expr)                                                                          \
    do {                                                                                           \
        extern void dwelui__test_assert_fail(const char *message, const char *file,                \
                                             u_int32_t line, const char *function);                \
        if (false == (expr)) {                                                                     \
            dwelui__test_assert_fail(#expr, __FILE__, __LINE__, __func__);                         \
                                                                                                   \
            return 1;                                                                              \
        }                                                                                          \
    } while (0)

#define TEST(fn_name)                                                                              \
    static int  fn_name(void);                                                                     \
                                                                                                   \
    static Test test_##fn_name                                                                     \
        __attribute__((used, section("tests"), aligned(__alignof__(Test)))) = {.name = #fn_name,   \
                                                                               .fn   = fn_name};   \
                                                                                                   \
    static int fn_name(void)

#endif // DWELUI_TEST_H

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
    extern void dwelui__test_register(const char *name, TestFn fn);                                \
    static void register_##fn_name(void) __attribute__((constructor));                             \
    static void register_##fn_name(void) {                                                         \
        dwelui__test_register(#fn_name, fn_name);                                                  \
    }                                                                                              \
    static int fn_name(void)

#endif // DWELUI_TEST_H

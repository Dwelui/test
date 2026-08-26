#ifndef DWELUI_TEST_H
#define DWELUI_TEST_H

#include <sys/types.h>

typedef void (*TestFn)(void);

typedef struct {
    const char *name;
    TestFn      fn;
} Test;

#define test_assert(expr)                                                                          \
    do {                                                                                           \
        extern void dwelui__test_assert_impl(bool condition, const char *expression,               \
                                             const char *file, u_int8_t line);                     \
                                                                                                   \
        dwelui__test_assert_impl((expr), #expr, __FILE__, __LINE__);                               \
    } while (0)

#define TEST(fn_name)                                                                              \
    static void       fn_name(void);                                                               \
                                                                                                   \
    static const Test test_##fn_name                                                               \
        __attribute__((used, section("tests"))) = {.name = #fn_name, .fn = fn_name};               \
                                                                                                   \
    static void fn_name(void)

#endif // DWELUI_TEST_H

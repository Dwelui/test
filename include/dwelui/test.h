#ifndef TEST_H
#define TEST_H

#include <sys/types.h>

typedef void (*TestFn)(void);

typedef struct {
    const char *name;
    TestFn      fn;
} Test;

void test_assert_impl(bool condition, const char *expression, const char *file, u_int8_t line);

#define test_assert(condition) test_assert_impl((condition), #condition, __FILE__, __LINE__)

#define TEST(fn_name)                                                                              \
    static void       fn_name(void);                                                               \
                                                                                                   \
    static const Test test_##fn_name                                                               \
        __attribute__((used, section("tests"))) = {.name = #fn_name, .fn = fn_name};               \
                                                                                                   \
    static void fn_name(void)

#endif // TEST_H

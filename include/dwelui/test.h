#ifndef TEST_H
#define TEST_H

typedef void (*TestFn)(void);

typedef struct {
    const char *name;
    TestFn      fn;
} Test;

#define TEST(fn_name)                                                                              \
    static void       fn_name(void);                                                               \
                                                                                                   \
    static const Test test_##fn_name                                                               \
        __attribute__((used, section("tests"))) = {.name = #fn_name, .fn = fn_name};               \
                                                                                                   \
    static void fn_name(void)

#endif // TEST_H

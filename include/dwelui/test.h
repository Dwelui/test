#ifndef TEST_H
#define TEST_H

#define TEST(domain, name)                                                                         \
    static void                              domain##_##name##_impl(void);                         \
    static void __attribute__((constructor)) domain##_##name##_register(void) {                    \
        test_add(#domain, #name, domain##_##name##_impl);                                          \
    }                                                                                              \
    static void domain##_##name##_impl(void)

typedef void (*TestCallback)(void);

void test_add(const char *domain, const char *name, TestCallback test);

#endif // TEST_H

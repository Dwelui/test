#include <dwelui/test.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#define TEST(domain, name)                                                                         \
    static void                              domain##_##name##_impl(void);                         \
    static void __attribute__((constructor)) domain##_##name##_register(void) {                    \
        test_add(#domain, #name, domain##_##name##_impl);                                          \
    }                                                                                              \
    static void domain##_##name##_impl(void)

typedef struct {
    const char  *domain;
    const char  *name;
    TestCallback callback;
} Test;

typedef struct {
    Test    *tests;
    u_int8_t count;
    u_int8_t capacity;
} TestList;

TestList testList;

static void __attribute__((constructor)) test_init() {
    testList.count = 0;
    testList.capacity = 64;
    testList.tests = malloc(sizeof(Test) * testList.capacity);
}

TEST(test, test) {
    printf("test_test_impl\n");
}

int main() {
    for (u_int8_t i = 0; i < testList.count; i++) {
        testList.tests[i].callback();
    }

    free(testList.tests);

    return 0;
}

void test_add(const char *domain, const char *name, TestCallback test) {
    if (testList.count >= testList.capacity) {
        return;
    }

    testList.tests[testList.count++] = (Test){.domain = domain, .name = name, .callback = test};

    return;
}

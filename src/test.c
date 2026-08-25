#include <dwelui/test.h>
#include <stdio.h>
#include <sys/types.h>

typedef struct {
    char         domain[32];
    char         name[64];
    TestCallback callback;
} Test;

typedef struct {
    Test     *tests;
    u_int16_t count;
} TestList;

void first_test() {
    printf("first_test\n");
}

int main() {
    test_add("test", "first_test", first_test);

    printf("Hello world!");

    return 0;
}

void test_add(const char *domain, const char *name, TestCallback test) {
    (void)domain;
    (void)name;

    test();

    return;
}

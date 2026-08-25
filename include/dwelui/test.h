#ifndef TEST_H
#define TEST_H

typedef void (*TestCallback)();

void test_add(const char *domain, const char *name, TestCallback test);

#endif // TEST_H

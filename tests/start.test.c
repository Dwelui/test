#include <dwelui/test.h>

TEST(failed_test, {
    test_assert(2 + 2 == 4);

    test_fail("this test will fail");
})

TEST(skipped_test, {
    test_skip("this test will be skipped");
    test_assert(2 + 2 != 4);

    test_assert(2 + 2 == 4);
})

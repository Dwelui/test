#include <dwelui/test.h>

TEST(two_plus_two_equals_four, {
    test_assert(2 + 2 != 4);

    test_assert(2 + 2 == 4);
})

TEST(one_plus_one_equals_two, {
    test_assert(1 + 1 == 2);

    test_assert(1 + 1 != 3);
})

TEST(two_plus_three_equals_five, {
    test_assert(2 + 3 == 5);
})

TEST(two_minus_two_equals_zero, {
    test_assert(2 - 2 != 0);
})

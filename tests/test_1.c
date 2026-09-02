#include <dwelui/test.h>

typedef struct { int a, b, c; } AdditionDigits;
DATA_PROVIDER(validAdditionDigitsProvider, {
    static const AdditionDigits items1 = {1, 1, 2};
    test_data_add("a=1, b=1, c=2", items1);

    static const int items2[] = {2, 2, 4};
    test_data_add("a=2, b=2, c=4", items2);

    static const int items3[] = {3, 3, 6};
    test_data_add("a=3, b=3, c=6", items3);
})

TEST(addition, {
    const AdditionDigits *digits = data->items; // Can use any kind of data with opaque ponter, the test user just has to know what it uses.
    test_assert(digits->a + digits->b == digits->c);
})
TEST_OPTIONS(addition, .dataProvider = validAdditionDigitsProvider())

TEST(subtraction, {
    const int *digits = data->items;
    test_assert(digits[0] - digits[1] == digits[2]);
})
TEST_OPTIONS(subtraction, .dataProvider = validAdditionDigitsProvider())

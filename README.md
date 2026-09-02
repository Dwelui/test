# libtest

## Syntax

The syntax this library is striving for should be pleasant and easy to use:

```c
TEST(test_name, {
    test_assert(2 + 2 == 4);
})
TEST_OPTIONS(test_name,
    TEST_GROUPS(group_1, group_2, group_3),
    TEST_DATA(data_provider_fn_name)
)
```

To achieve this syntax discovery and test registration mechanism is essential. There are few realistic approaches:

- Platform specific implementation. The linker magic for GCC/Clangd is one of the specific implementations (currently used).
- `Generated registry`. Very simple and portable, generates additional files. Macro can generate special signature that generation script can discover and register.

Would like to move towards `Generated registry` in the future.

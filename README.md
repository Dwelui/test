# test

## Syntax

The syntax this library is striving for should be pleasant and easy to use, look at "tests" directory for examples.

## Goals

- [x] Memory allocation error on file without .test.
- [ ] Output graceful error for `test_data_get` when `__data` is `nullptr`, used without data provider
- [ ] Move to `Generated registry` from `Platform specific` solutions for auto registered tests
- [ ] Detect when `test` did not run any assertions. Mark it as empty
- [ ] Return correct status to `environment` when `tests` fail.
- [x] Add `Data Providers`
- [x] Rename `Data Providers` to `Test Data Providers`
- [x] Solve `Test Data Providers` name collision over files
- [ ] Add filtering by file path fragment
- [ ] Support multiple output formats
- [x] Change output by coloring each tests name as gray/light gray and leaving only status color coded.
- [x] Total should show: test totals and test result total which works with `data providers`.

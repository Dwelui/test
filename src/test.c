#include <dwelui/test.h>
#include <stdint.h>
#include <sys/types.h>

extern const Test __start_tests[];
extern const Test __stop_tests[];

int               main() {
    for (const Test *test = __start_tests; test < __stop_tests; ++test) {
        test->fn();
    }

    return 0;
}

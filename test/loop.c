// RUN: %symcc -O2 %s -o %t
// RUN: echo -ne "\x05\x00\x00\x00" | %t 2>&1 | %filecheck %s
// RUN: %symcc -m32 -O2 %s -o %t_32
// RUN: echo -ne "\x05\x00\x00\x00" | %t_32 2>&1 | %filecheck %s
//
// Make sure that our instrumentation works with back-jumps. Also, test support
// for 128-bit integers (if available).
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#ifdef __SIZEOF_INT128__
#define MYINT __int128
#else
#define MYINT int64_t
#endif

int fac(int x) {
    MYINT result = 1;

    // SIMPLE-COUNT-5: Found diverging input
    // SIMPLE-NOT: Found diverging input
    // QSYM-COUNT-5: New testcase
    for (MYINT i = 2; i <= x; i++)
        result *= i;

    return result;
}

int main(int argc, char* argv[]) {
    int x;
    if (read(STDIN_FILENO, &x, sizeof(x)) != sizeof(x)) {
        printf("Failed to read x\n");
        return -1;
    }
    printf("%d\n", fac(x));
    return 0;
}

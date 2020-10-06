#include <stdio.h>

void foo1(int n) {
    int x = 9;
}

void foo2(int n) {
    foo1(2);
}

void foo3(int n) {
    printf("%d", n);
}

int main() {
    foo1(1);
    foo2(2);
    foo3(3);
    printf("hello %d some string %d another string %d goodbye", 1,2,3);
    if (1 == 1) {
        for (long long i = 0; i < 5; ++i) {
            printf("%lld hello %.10f", i, 0.333);
        }
    }
    return 0;
}

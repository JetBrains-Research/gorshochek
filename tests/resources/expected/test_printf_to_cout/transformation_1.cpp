#include <iomanip>
#include <iostream>
#include <stdio.h>

void foo1(int n) {
    int x = 9;
}

void foo2(int n) {
    foo1(2);
}

void foo3(int n) {
    std::cout << n;
}

int main() {
    foo1(1);
    foo2(2);
    foo3(3);
    std::cout << 1 << 2 << 3;
    if (1 == 1) {
        for (long long i = 0; i < 5; ++i) {
            std::cout << i << std::fixed << std::setprecision(10) << 0.333;
        }
    }
    return 0;
}

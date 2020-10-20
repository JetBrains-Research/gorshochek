#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <iostream>
#include <cstdio>

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
    std::cout << "hello " << 1 << " some string " << 2 << " another string " << 3 << " goodbye";
    if (1 == 1) {
        for (long long i = 0; i < 5; ++i) {
            std::cout << i << " hello " << std::fixed << std::setprecision(10) << 0.333;
        }
    }
    return 0;
}

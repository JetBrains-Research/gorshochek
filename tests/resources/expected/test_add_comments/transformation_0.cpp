#include <bits/stdc++.h>
using namespace std;

void foo(int n) {
    cout << "hello";
    int i = 0;
    int j = 0;
    for (int i = 0; i < n; i++)
        j += i + 1;
    while (j - 1 > 0) {
        i++;
        if (i == n)
            break;
        else
            i++;
    }
}

int main() {
    foo(2);
    return 0;
}
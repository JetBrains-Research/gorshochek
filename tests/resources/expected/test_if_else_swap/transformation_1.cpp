#include <bits/stdc++.h>
using namespace std;

int foo(int n) {
    cout << "hello";
    if (!(n == 1)) int k = 4;
 else
        {
        int k = 0;
        int j = 0;
    }
    if (!(n > 1)) {
        int k = 5;
        if (k < 5) {
            k--;
        }
        return k;
    } else {
        int k = n + 5;
        return 1;
    }
}

int main() {
    int x = foo(1);
    if (x == 2) {
        return 1;
    } else if (x == 6) {
        return 0;
    } else if (x >= 1) {
        int a;
        if ( !((x < 3 && x > 1) || (x == 0)) )
            return 0;
        else
            a = 4;
        return a;
    } else {
        return 2;
    }
    return 0;
}

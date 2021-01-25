#include <bits/stdc++.h>
using namespace std;

int foo(int n) {
    cout << "hello";
    if (n == 1) {
        int k = 0;
        int j = 0;
    } else
        int k = 4;

    if (n > 1) {
        int k = n + 5;
        return 1;
    } else {
        int k = 5;
        if (k < 5) {
            k--;
        }
        return k;
    }
}

int main() {
    int big_var = 0;
    int x = foo(1);
    int bigger_var = x - 1;
    if (big_var == bigger_var) {
        return 1;
    } else {
        x = 5;
    }
    if (x == 6) {
        return 0;
    } else if (x >= 1) {
        int a;
        if ( (x < 3 && x > 1) || (x == 0) )
            a = 4;
        else
            return 0;
        return a;
    } else {
        return 2;
    }
    vector<int> v = {1, 2, 3};
    if(foo(x) == 1) {
        int tmp_sz = 11;
        for(auto &z : v) {
            if (foo(3) == 0) {
                v.push_back(x + 2);
            } else {
                v[0] = max(14, x);
            }
        }
    } else {
        for(auto &z : v) {
            if (foo(7) == 0) {
                v.push_back(x);
            } else {
                v[0] = max(1, x);
            }
        }
    }
    for (long long k = 0; k < 30; k++)
        if (1 & (1 << k)) {
            if (k == -1)
                continue;
            else
                x += 1 << k;
        }
    return 0;
}

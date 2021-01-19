#include <bits/stdc++.h>
using namespace std;

void test_do_math(int *test_x) {
    cout << "hello";
    *test_x += 5;
}

class Class {
public:
    void test_ffff(int &test_S) {
        int test_totalset = 33;
        for(int test_i=0; test_i<test_S;){
            int test_count = test_totalset;
            test_i += test_count;
        }
    }
};

int test_join(int test_x) {
    int test_z = test_x + 4;
    auto test_d = Class();
    test_do_math(&test_z);
    if (test_z < 1) {
        test_d.test_ffff(test_z);
        return test_x - 2;
    } else {
        test_do_math(&test_x);
        return 0;
    }
    int test_some_val;
}

void test_tmppp(int test_k) {
    test_k += 1;
}

template<class T> int test_chkmax(T &test_a, T test_b) {
    if (test_b > test_a) {
        test_a = test_b;
        return 1;
    }
    return 0;
}

int main(void) {
    int test_result = -1.0, test_val = 4;
    int test_k0 = 1;
    std::vector<int> test_lam = {test_k0};
    int test_x;
    int test_omega = test_join(test_result);
    test_do_math(&test_val);
    test_x = 0;
    test_do_math(&test_x);
    if (test_join(test_val) > 3) {
        test_do_math(&test_omega);
    }
    auto test_o = Class();
    test_o.test_ffff(test_val);
    test_o.test_ffff(test_result);
    test_chkmax(test_x, test_k0);
    test_chkmax(test_result, test_result);
    return test_result;
}
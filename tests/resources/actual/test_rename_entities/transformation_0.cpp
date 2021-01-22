#include <bits/stdc++.h>
using namespace std;

void do_math(int *x) {
    cout << "hello";
    *x += 5;
}

class Class {
public:
    void ffff(int &S) {
        int totalset = 33;
        for(int i=0; i<S;){
            int count = totalset;
            i += count;
        }
    }
};

int join(int x) {
    int z = x + 4;
    auto d = Class();
    do_math(&z);
    if (z < 1) {
        d.ffff(z);
        return x - 2;
    } else {
        do_math(&x);
        return 0;
    }
    int some_val;
}

void tmppp(int k) {
    k += 1;
}

struct event {
    int x, type, id;
};

bool operator<(event a, event b) {
    return make_pair(a.x, a.type) < make_pair(b.x, b.type);
}

template<class T> int chkmax(T &a, T b) {
    if (b > a) {
        a = b;
        return 1;
    }
    return 0;
}

class Event {
public:
    int x, type, id;

    Event() = default;

    Event(int x, int type, int id) : x(x), type(type), id(id) {}
};

bool cmp(int a, int b) {
    if (a % 2 == 0) {
        return a < b;
    }
    return a > b;
}

int main(void) {
    int result = -1.0, val = 4;
    int k0 = 1;
    std::vector<int> lam = {k0};
    int x;
    int omega = join(result);
    do_math(&val);
    x = 0;
    do_math(&x);
    if (join(val) > 3) {
        do_math(&omega);
    }
    auto o = Class();
    o.ffff(val);
    o.ffff(result);
    auto ev = Event();
    chkmax(x, k0);
    chkmax(result, result);
    vector<int> vector_int = {1, 2, 3, 4};
    sort(vector_int.begin(), vector_int.end(), cmp);
    return result;
}
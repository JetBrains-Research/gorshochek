void math_(int *xi) {
    *xi += 5;
}

class Class {
public:
    void f(int &S1) {
        int tt = 33;
        for(int j=0; j<S1;){
            int c = tt;
            j += c;
        }
    }
};

int jo(int xi) {
    int zi = xi + 4;
    auto doo = Class();
    math_(&zi);
    if (zi < 1) {
        doo.f(zi);
        return xi - 2;
    } else {
        math_(&xi);
        return 0;
    }
}


int main(void) {
    int res = -1, value = 4;
    int om = jo(res);
    math_(&value);
    int xi = 0;
    math_(&xi);
    if (jo(value) > 3) {
        math_(&om);
    }
    auto oo = Class();
    oo.f(value);
    oo.f(res);
    return res;
}
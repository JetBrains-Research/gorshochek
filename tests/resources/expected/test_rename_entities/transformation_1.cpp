void v(int *x) {
    *x += 5;
}

class Class {
public:
    void w(int &S) {
        int totalset = 33;
        for(int i=0; i<S;){
            int count = 0;
            i += count;
        }
    }
};

int wu_r(int x) {
    int z = x + 4;
    auto d = Class();
    v(&z);
    if (z < 1) {
        d.w(z);
        return x - 2;
    } else {
        v(&x);
        return 0;
    }
}


int main(void) {
    int result = -1, val = 4;
    int omega = wu_r(result);
    v(&val);
    int x = 0;
    v(&x);
    if (wu_r(val) > 3) {
        v(&omega);
    }
    auto o = Class();
    o.w(val);
    o.w(result);
    return result;
}
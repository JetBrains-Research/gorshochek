void do_math(int *x) {
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
}


int main(void) {
    int result = -1, val = 4;
    int omega = join(result);
    do_math(&val);
    int x = 0;
    do_math(&x);
    if (join(val) > 3) {
        do_math(&omega);
    }
    auto o = Class();
    o.ffff(val);
    o.ffff(result);
    return result;
}
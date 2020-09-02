void ai(int *x) {
    *x += 5;
}

class Class {
public:
    void ddk(int &S) {
        int totalset = 33;
        for(int i=0; i<S;){
            int count = 0;
            i += count;
        }
    }
};

int j(int x) {
    int z = x + 4;
    auto d = Class();
    ai(&z);
    if (z < 1) {
        d.ddk(z);
        return x - 2;
    } else {
        ai(&x);
        return 0;
    }
}


int main(void) {
    int result = -1, val = 4;
    int omega = j(result);
    ai(&val);
    int x = 0;
    ai(&x);
    if (j(val) > 3) {
        ai(&omega);
    }
    auto o = Class();
    o.ddk(val);
    o.ddk(result);
    return result;
}
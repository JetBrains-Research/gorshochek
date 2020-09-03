void c_m(int *a) {
    *a += 5;
}

class Class {
public:
    void bp_e(int &k) {
        int j = 33;
        for(int w=0; w<k;){
            int sd_y = j;
            w += sd_y;
        }
    }
};

int kh_z(int qo_tj) {
    int zr = qo_tj + 4;
    auto dx = Class();
    c_m(&zr);
    if (zr < 1) {
        dx.bp_e(zr);
        return qo_tj - 2;
    } else {
        c_m(&qo_tj);
        return 0;
    }
}


int main(void) {
    int h = -1, ge = 4;
    int a_mb = kh_z(h);
    c_m(&ge);
    int q = 0;
    c_m(&q);
    if (kh_z(ge) > 3) {
        c_m(&a_mb);
    }
    auto af_io = Class();
    af_io.bp_e(ge);
    af_io.bp_e(h);
    return h;
}
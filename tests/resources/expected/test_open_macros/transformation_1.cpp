#define BIG 10000
#define ADD(x, y) (x) + (y)
#define MIN(a,b) ((a)<(b)?(a):(b))
#define FOR(x,to) for(x=0;x<(to);x++)

void hello(int x) {
    (x) + (x);
}


int main() {
    int x = (((10000) + (10000)) < (10000) ? ((10000) + (10000)) : (10000));
    for (x = 0; x < (10); x++) {
        break;
    }
    int i = 0, j = 1, k = 5, pre = 7;
    for (x = 0; x < (10); x++) {
        for (i = 0; i < (256); i++)
            if (i % 2 == 1)
                j += k - pre;
        pre = k + j;
        if (k - j < i) {
            break;
        }
    }
    hello(2);
    return 0;
}

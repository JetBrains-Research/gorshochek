#define BIG 10000
#define ADD(x, y) (x) + (y)
#define MIN(a,b) ((a)<(b)?(a):(b))
#define FOR(x,to) for(x=0;x<(to);x++)

void hello(int x) {
    ADD(x, x);
}

int main(void) {
    int x = MIN(ADD(BIG, BIG), BIG);
    FOR(x, 10) {
        break;
    }
    int i =0, j = 1, k = 5, pre = 7;
    FOR(x, 10) {
        FOR(i, 256) if (i % 2 == 1) j += k - pre;
        pre = k + j;
        if (k - j < i) {
            break;
        }
    }
    hello(2);
    return 0;
}
void a(int *x) {
    int f;
    while(1) {
        f = 5;
    }

    while(f < 5)
        f++;
}

int main(void) {
    int x = 6;
    a(&x);
    while(1);
    int k = 0;
    while(k);
    return 0;
}
void a(int *x) {
    int f;
    for ( ; 1; ) {
        f = 5;
    }

    for ( ; f < 5; ) f++;
}

int main(void) {
    int x = 6;
    a(&x);
    for ( ; 1; ) ;
    int k = 0;
    for ( ; k; ) ;
    return 0;
}
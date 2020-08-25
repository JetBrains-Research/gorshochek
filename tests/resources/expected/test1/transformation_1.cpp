#define MOD 100

void foo1(int n) {
    for (int i = 0; i < n; i++)
        int j = 0;
}

void foo2(int n) {
    int i = 0;
    while (1) {
        int j = 0;
        i++;
        if (i == n)
            break;
        else
            i++;
    }
}

void foo3(int n) {
    int i = 0;
    while (i < n) {
        int j = 0;
        i++;
    }
}
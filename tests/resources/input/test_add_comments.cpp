void foo(int n) {
    int i = 0;
    int j = 0;
    for (int i = 0; i < n; i++)
        j += i + 1;
    while (j - 1 > 0) {
        i++;
        if (i == n)
            break;
        else
            i++;
    }
}
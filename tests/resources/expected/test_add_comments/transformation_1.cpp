void foo(int n) {
    int i = 0;
    int j = 0;
    /* 'for' begin */
    for (int i = 0; i < n; i++)
        /* 'for' inside */j += i + 1;
    /* 'while' begin */
    while (j - 1 > 0) /* 'while' inside */
    {
        i++;
        /* 'if' begin */
        if (i == n)
            /* 'if' inside */
            break;
        else
            i++;
    }
}
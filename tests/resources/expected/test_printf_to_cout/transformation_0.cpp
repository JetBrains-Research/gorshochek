#include <stdio.h>
#include <iostream>
#include <bits/stdc++.h>
using namespace std;
#include <cstdio>

void foo1(int n) {
    int x = 9;
}

void foo2(int n) {
    foo1(2);
}

void foo3(int n) {
    printf("%d", n);
}

int main() {
    int ti,tn;
    scanf("%d",&tn);
    int x,i,j,ans,n,p;
    for(ti=0;ti<tn;ti++) {
        scanf("%d",&n);
        vector<bool> a(1010,0);
        vector<bool> b(1010,0);
        for(i=0;i<n;i++) {
            scanf("%d",&p);
            a[p] = 1;
        }
        for(i=0;i<n;i++) {
            scanf("%d",&p);
            b[p] = 1;
        }

        for(i=0;i<1010;i++) {
            if(a[i]==1) {
                printf("%d",i);
                break;
            }
        }
        i++;
        for(;i<1010;i++) {
            if(a[i]==1) {
                printf(" %d",i);
            }
        }
        printf("\n");

        for(i=0;i<1010;i++) {
            if(b[i]==1) {
                printf("%d",i);
                break;
            }
        }
        i++;
        for(;i<1010;i++) {
            if(b[i]==1) {
                printf(" %d",i);
            }
        }

        printf("\n");
    }
    std::ios_base::sync_with_stdio(false);
    foo1(1);
    foo2(2);
    foo3(3);
    printf("hello %d some string %d another string %d goodbye", 1,2,3);
    if (1 == 1) {
        for (long long i = 0; i < 5; ++i) {
            printf("%lld hello %.10f", i, 0.333);
        }
    }
    return 0;
}

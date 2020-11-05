#include <bits/stdc++.h>
using namespace std;

int external() {
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

    return 0;
}

void foo1(int n) {
    int x = 9;
}

void foo2(int n) {
    foo1(2);
}

void foo3(int n) {
    foo1(3);
    foo2(4);
}

void t();

int main() {
    foo1(1);
    foo2(2);
    foo3(3);
    t();
    external();
    return 0;
}

void t() {
    char d;
}

void foo() {
    int x;
}
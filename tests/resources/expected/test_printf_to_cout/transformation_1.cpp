#include <iomanip>
#include <iostream>
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
    std::cout << n;
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
                std::cout << i;
                break;
            }
        }
        i++;
        for(;i<1010;i++) {
            if(a[i]==1) {
                std::cout << " " << i;
            }
        }
        std::cout << "\n";

        for(i=0;i<1010;i++) {
            if(b[i]==1) {
                std::cout << i;
                break;
            }
        }
        i++;
        for(;i<1010;i++) {
            if(b[i]==1) {
                std::cout << " " << i;
            }
        }

        std::cout << "\n";
    }
    
    foo1(1);
    foo2(2);
    foo3(3);
    std::cout << "hello " << 1 << " some string " << 2 << " another string " << 3 << " goodbye";
    if (1 == 1) {
        for (long long i = 0; i < 5; ++i) {
            std::cout << i << " hello " << std::fixed << std::setprecision(10) << 0.333;
        }
    }
    return 0;
}

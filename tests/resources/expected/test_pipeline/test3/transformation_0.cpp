void bar1(int n) { n++; }

void bar2(int n) { n += 1; }

void bar3(int n) { ++n; }

void bar4(int n) { n = n + 10; }

int main(){
    bar1(1);
    bar2(2);
    bar3(3);
    bar4(4);
    return 0;
}
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
    return 0;
}

void t() {
    char d;
}

void foo() {
    int x;
}
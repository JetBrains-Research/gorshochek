void foo1(int n);
void foo2(int n);
void foo3(int n);

void foo3(int n) {
    int i = 0;
}

void foo2(int n) {
    foo3(2);
}

void foo1(int n) {
    foo2(3);
    foo3(4);
}

int main() {
    foo1(1);
    foo2(2);
    foo3(3);
    return 0;
}
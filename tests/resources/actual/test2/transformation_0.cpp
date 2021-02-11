void tmp1(int n) { int a = n; }

void tmp2(int n) { auto a = n; }

int main(){
    tmp1(1);
    tmp2(2);
    return 0;
}
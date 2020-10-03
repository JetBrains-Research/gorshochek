void a(int *x) {
    int i = 0;
    while (i < 4) {
        int d = 4;
    	++i; 
    }

    int k;
    k =   0;
    while (k <   6) {
        if(k == 4) {
            ++k; continue;
        }
        if (k == 5)
            ++k; continue;
        int d = 4;
        while (d<5) {
            int z;	
            d--; 
            }
    	++k; 
    }
}

int main(void) {
    int val = 4;
    int i=0;
    while (i < 4) {
        int k = 5;	
        i++; 
        }
    a(&val);
    int x = 0;
    int s = 5;
    while (1) {
        if (s == 6) {
            break;
        }
    	s += 1; 
    }
    a(&x);
    while (x != 0) ;
    while (1) ;
    return 0;
}
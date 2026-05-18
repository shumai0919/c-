#include <stdio.h>

int main(void) {
    int a;
    int b0 = 0, 
        b1 = 0, 
        b2 = 0, 
        b3 = 0, 
        b4 = 0, 
        b5 = 0, 
        b6 = 0, 
        b7 = 0;
    printf("入力 ");
    scanf("%d", &a);
    
    a %= 256;
    int n = a;
    if (a >= 1) {
        if(n % 2 == 1) b0 = 1;
        n /= 2;
    }
    if (a >= 2) {
        if(n % 2 == 1) b1 = 1;
        n /= 2;
    }
    if (a >= 4) {
        if(n % 2 == 1) b2 = 1;
        n /= 2;
    }
    if (a >= 8) {
        if(n % 2 == 1) b3 = 1;
        n /= 2;
    }
    if (a >= 16) {
        if(n % 2 == 1) b4 = 1;
        n /= 2;
    }
    if (a >= 32) {
        if(n % 2 == 1) b5 = 1;
        n /= 2;
    }
    if (a >= 64) {
        if(n % 2 == 1) b6 = 1;
        n /= 2;
    }
    if (a >= 128) {
        if(n % 2 == 1) b7 = 1;
        n /= 2;
    }
    // printf("%d%d%d%d%d%d%d%d\n", b7, b6, b5, b4, b3, b2, b1, b0);

    if (b7 == 1) printf("UP ");
    if (b6 == 1) printf("DOWN ");
    if (b5 == 1) printf("LEFT ");
    if (b4 == 1) printf("RIGHT ");
    if (b3 == 1) printf("B ");
    if (b2 == 1) printf("A ");
    printf("\n");
    return 0;
}
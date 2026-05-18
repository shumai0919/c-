#include <stdio.h>
int main(void){
    // Your code here!
    int w;
    printf("整数を入力\n");
    scanf("%d", &w);
    switch (w) {
        case 1:
            printf("Jan.\n");
            break;
        case 2:
            printf("Feb.\n");
            break;
        case 3:
            printf("Mar.\n");
            break;
        case 4:
            printf("Apr.\n");
            break;
        case 5:
            printf("May.\n");
            break;
        case 6:
            printf("Jun.\n");
            break;
        case 7:
            printf("Jul.\n");
            break;
        case 8:
            printf("Aug.\n");
            break;
        case 9:
            printf("Sep.\n");
            break;
        case 10:
            printf("Oct.\n");
            break;
        case 11:
            printf("Nov.\n");
            break;
        case 12:
            printf("Dec.\n");
            break;
        default:
            printf("入力文字はA,B,C以外です。\n");
    }
    return 0;
}

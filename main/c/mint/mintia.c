#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<termios.h>
#include<unistd.h>
#include<fcntl.h>

#define MAXCLOG 3

int kbhit(void) {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    
    return 0;
}

void wait_with_kbhit(int ms) {
    int elapsed = 0;
    while (elapsed < ms) {
        if (kbhit()) {
            int ch = getchar();
            if (ch == ' ') {
                tcflush(STDIN_FILENO, TCIFLUSH);
                break;
            }
        }
        usleep(10000);
        elapsed += 10;
    }
}

int getRandom(int min,int max){
//minからmaxまでの乱数
    return min+rand()%(max-min+1);
}

double getRandomDouble(){
//0.0から1.0の乱数
    return (double)rand()/RAND_MAX;
}

void mintia(){
    system("clear");
    printf("\n\n --------------------------------------------\n");
    printf("|   __  __ ___ _   _ _____ ___     _         |\n");
    printf("|  |  \\/  |_ _| \\ | |_   _|_ _|   / \\        |\n");
    printf("|  | |\\/| || ||  \\| | | |  | |   / _ \\       |\n");
    printf("|  | |  | || || |\\  | | |  | |  / ___ \\      |\n");
    printf("|  |_|  |_|___|_| \\_| |_| |___|/_/   \\_\\     |\n");
    printf("|                                            |\n");
    printf("|  |----------|                              |\n");
    printf(" -------------------------------------------- \n");
}

void showMnt(int num){
    int j,entercount=0,showcount=0;
    for(int i=0;i<2;i++){
        if(i==0){
            printf("ｶｼｬｶｼｬ...\n");
            printf("---  ___  ---  ___  ---  ___  ---  ___  ---\n");
            printf(" --------------------------------------------\n");
            printf("|   __  __ ___ _   _ _____ ___     _         |\n");
            printf("|  |  \\/  |_ _| \\ | |_   _|_ _|   / \\        |\n");
            printf("|  | |\\/| || ||  \\| | | |  | |   / _ \\       |\n");
            printf("|  | |  | || || |\\  | | |  | |  / ___ \\      |\n");
            printf("|  |_|  |_|___|_| \\_| |_| |___|/_/   \\_\\     |\n");
            printf("|                                            |\n");
            printf("|  |----------|                              |\n");
            printf(" -------------------------------------------- \n");
            printf("---  ___  ---  ___  ---  ___  ---  ___  ---\n");
            wait_with_kbhit(500);
        }
        if(i==1){
            system("clear");
            printf("\n");
            printf("\n");
            printf(" --------------------------------------------\n");
            printf("|   __  __ ___ _   _ _____ ___     _         |\n");
            printf("|  |  \\/  |_ _| \\ | |_   _|_ _|   / \\        |\n");
            printf("|  | |\\/| || ||  \\| | | |  | |   / _ \\       |\n");
            printf("|  | |  | || || |\\  | | |  | |  / ___ \\      |\n");
            printf("|  |_|  |_|___|_| \\_| |_| |___|/_/   \\_\\     |\n");
            printf("|                                            |\n");
            printf("|  |----------|                              |\n");
            printf(" --            ------------------------------ \n");
            printf("   |__________|\n");
            if(num>0){
                do{
                    int batch=0;
                    for(batch=0;batch<3&&showcount<num;batch++,showcount++){
                        printf("  ______     ");
                    }
                    printf("\n");
                    for(int k=0;k<batch;k++){
                        printf(" /      \\    ");
                    }
                    printf("\n");
                    for(int k=0;k<batch;k++){
                        printf("|        |   ");
                    }
                    printf("\n");
                    for(int k=0;k<batch;k++){
                        printf(" \\______/    ");
                    }
                    printf("\n\n");
                }while(showcount<num);
                
            }else{
                printf("\n\n\n\n\n");
            }
        }
    }
}

int DropNumberMnt(int* tumarido,int* tumarityu,int* tamari){
    double r=(double)rand()/RAND_MAX;
    int temp;
    if(*tumarityu==0){
        if(r<0.2){
            //詰まりを発生
            *tumarityu=1;
            (*tumarido)++;
            (*tamari)++;
            return 0;
        }else{
            r=(double)rand()/RAND_MAX;
            if(r<0.02){
                return 2;
            }else{
                return 1;
            }
        }
    }else{
        if(r<0.6&&*tumarido<MAXCLOG){
            //詰まり続ける
            (*tumarido)++;
            (*tamari)++;
            return 0;
        }else{
            //詰まりを排出
            temp=*tamari;
            *tamari=*tumarido=0;
            if(*tumarido==0){
                *tumarityu=0;
            }
            return temp+1;
        }
    }
}

int main(void){
    srand(time(NULL));
    int clogging=0,blocked=0,stock=0,drops;
    mintia();
    printf("スペースキーで出す");
    while(1){
        if(kbhit()){
            int ch=getchar();
            switch(ch){
                case ' ':
                    system("clear");
                    drops=DropNumberMnt(&clogging,&blocked,&stock);
                    showMnt(drops);
                    //printf("%d %d %d",clogging,blocked,stock);
                    printf("排出：%d個\n",drops);
                    break;
                case 'q':
                    printf("\n\n    終了\n");
                    return 0;
                default:
                    mintia();
                    printf("\n\n    スペース：排出\n    Q：終了");
            }
        }
        usleep(10000);
    }
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#define WAR_FUND 300000

/*void showPossessionCards(int *possession){
    int length = sizeof(possession) / sizeof(int);
    for(int k=0;k<length;k++){
        printf("%d ",possession[k]);
    }
}*/

char getCardMark(int card){
    int num_mark;
    char mark;
    num_mark = card / 13;
    if(num_mark == 0){
        mark = 'S';
    }else if(num_mark == 1){
        mark = 'H';
    }else if(num_mark == 2){
        mark = 'D';
    }else{
        mark = 'C';
    }
    return mark;
}

int contains(int arr[],int size,int target){
    for(int i = 0; i < size; i++){
        if(arr[i] == target){
            return 1;
        }
    }
    return 0;
}

int getCardNum(int card){
    if(card % 13 == 0){
        return 13;
    }
    return card % 13;
}

int main(void){
    int player_scoretotal = 0,dealer_scoretotal = 0,hit_count = 0,betamount = 0,return_amount = 0,player_money = WAR_FUND,player_cardhand,score_drawingcard,num_selection,ace_selection;
    float return_amount_magnifi;
    char win_or_loss = 'n',str_selection = 'h',cardMark;
    int player_owncard[12] = {0},dealer_owncard[12] = {0},cardstorage_shdc[52] = {0};
    srand((unsigned)time(NULL));

    while(1){

        for(int i = 0;i < 52;i++){
            cardstorage_shdc[i] = i+1;
        }
                player_scoretotal = dealer_scoretotal = 0;
        for(int i = 0;i < 12; i++){
            player_owncard[i] = dealer_owncard[i] = 0;
        }
        str_selection = 'n';
        ace_selection = '0';
        win_or_loss = 'n';
        player_cardhand = 0;
        return_amount = 0;
        hit_count = 0;

        if(player_money < 100){
            printf("$100以上でゲームプレイ可能です\n");
            usleep(1000000);
            system("clear");
            printf("\n\n\n    YOU DIED\n\n\n");
            usleep(2000000);
            system("clear");
            exit(0);
        }
        int bet_entry = 0;
        do{
            printf("所持金：$%d\n",player_money);
            printf("ベット額を入力してください > ");
            scanf("%d",&betamount);
            if(betamount < 100){
                printf("ベット額は$100からです。\n");
            }else if(betamount > player_money){
                printf("所持金が足りません。\n");
            }else{
                printf("$%d賭けました。\n",betamount);
                return_amount = betamount;
                player_money -= betamount;
                bet_entry = 1;
                printf("所持金：$%d\n",player_money);
            }
        }while(bet_entry == 0);

        printf("\nプレイヤー、ディーラーに2枚カードを配ります...\n");
        usleep(1000000);
        int rand1 = 0,rand2 = 0;
        for(int i = 0;i < 2;i++){
            for(int j = 0;j < 2;j++){
                do{
                    rand1 = rand() % 14;
                }while(cardstorage_shdc[rand1] == 0);
                if(i == 0){
                    player_owncard[j] = cardstorage_shdc[rand1];
                    if(getCardNum(cardstorage_shdc[rand1]) == 1 && player_scoretotal+10 <= 21){
                        do{
                            printf("Aceを引きました。(1:'1'としてカウント、2:'11'としてカウント) ");
                            if(j == 1){
                                printf("1枚目: %d",getCardNum(player_owncard[0]));
                            }
                            printf(" > ");
                            scanf("%d",&ace_selection);
                        }while(ace_selection != 1 && ace_selection != 2);
                        if(ace_selection == 1){player_scoretotal += 1;} else{player_scoretotal += 11;}
                    }else if(getCardNum(cardstorage_shdc[rand1]) >= 11 && getCardNum(cardstorage_shdc[rand1]) <= 13){
                        player_scoretotal += 10;
                    }else{
                        player_scoretotal += getCardNum(cardstorage_shdc[rand1]);
                    }
                    cardstorage_shdc[rand1] = 0;
                }else{
                    dealer_owncard[j] = cardstorage_shdc[rand1];
                    if(getCardNum(cardstorage_shdc[rand1]) == 1 && dealer_scoretotal+10 <= 21){
                        dealer_scoretotal += 11;
                    }else if(getCardNum(cardstorage_shdc[rand1]) >= 11 && getCardNum(cardstorage_shdc[rand1]) <= 13){
                        dealer_scoretotal += 10;
                    }else{
                        dealer_scoretotal += getCardNum(cardstorage_shdc[rand1]);
                    }
                    cardstorage_shdc[rand1] = 0;
                }
            
            }
        }
        
        printf("プレイヤーの手札: %d, %d (合計: %d)\n",getCardNum(player_owncard[0]),getCardNum(player_owncard[1]),player_scoretotal);
        printf("ディーラーの手札: %d, ? (合計: ?)\n",getCardNum(dealer_owncard[0]));
        printf("\n");
        hit_count = 0;
        printf("- プレイヤーのターン -\n");
        while(str_selection != 's'){
            do{
                rand1 = rand() % 52;
            }while(cardstorage_shdc[rand1] == 0);
            do{
                if(hit_count == 0){
                    printf("'Hit', 'Stand' or 'DoubleDown?' (h/s/d) > ");
                }else{
                    printf("'Hit' or 'Stand'? (h/s) > ");
                }
                scanf(" %c",&str_selection);
            }while(str_selection != 'h' && str_selection != 's' && str_selection != 'd');
            if(str_selection == 'h' || str_selection == 'd'){
                player_owncard[2 + hit_count] = cardstorage_shdc[rand1];
                if(getCardNum(cardstorage_shdc[rand1]) == 1 && player_scoretotal+10 <= 21){
                    do{
                        printf("Aceを引きました。(1:'1'としてカウント、2:'11'としてカウント) > ");
                        scanf("%d",&ace_selection);
                    }while(ace_selection != 1 && ace_selection != 2);
                    if(ace_selection == 1){player_scoretotal += 1;} else{player_scoretotal += 11;}
                }else if(getCardNum(cardstorage_shdc[rand1]) >= 11 && getCardNum(cardstorage_shdc[rand1]) <= 13){
                    player_scoretotal += 10;
                }else{
                    player_scoretotal += getCardNum(cardstorage_shdc[rand1]);
                }
                cardstorage_shdc[rand1] = 0;
                hit_count++;
                printf("手札: ");
                for(int k=0;k<2+hit_count;k++){
                    printf("%d,",getCardNum(player_owncard[k]));
                }
                printf("(合計: %d)\n", player_scoretotal);
                if(player_scoretotal > 21){
                    printf("バーストした。\n");
                    win_or_loss = 'l';
                    break;
                }
                if(str_selection == 'd'){
                    betamount *= 2;
                    break;
                }
            }
        }
        hit_count = 0;
        if(win_or_loss != 'l'){
            printf("\n- ディーラーのターン -\n");
            usleep(1000000);
        }
        while(dealer_scoretotal < 17){
            if(win_or_loss == 'l'){break;}
            do{
                rand1 = rand() % 52;
            }while(cardstorage_shdc[rand1] == 0);
            dealer_owncard[2 + hit_count] = cardstorage_shdc[rand1];
            if(getCardNum(cardstorage_shdc[rand1]) == 1 && dealer_scoretotal+10 <= 21){
                dealer_scoretotal += 11;
            }else if(getCardNum(cardstorage_shdc[rand1]) >= 11 && getCardNum(cardstorage_shdc[rand1]) <= 13){
                dealer_scoretotal += 10;
            }else{
                dealer_scoretotal += getCardNum(cardstorage_shdc[rand1]);
            }
            cardstorage_shdc[rand1] = 0;
            hit_count++;
            if(dealer_scoretotal > 21){
                printf("ディーラーがバーストしました 。\n");
                win_or_loss = 'w';
                break;
            }
        }
        if(win_or_loss == 'n'){
            usleep(1000000);
            printf("ディーラーのターンが終了しました。\n\n");
        }

        if(win_or_loss == 'n'){
            printf("点数を比較します...\n");
            printf("プレイヤーの合計: %d\n",player_scoretotal);
            printf("ディーラーの合計: %d\n",dealer_scoretotal);
            if(player_scoretotal > dealer_scoretotal){
                win_or_loss = 'w';
            }else if(player_scoretotal < dealer_scoretotal){
                win_or_loss = 'l';
            }else{
                win_or_loss = 'd';
            }
        }
        printf("\n");
        if(win_or_loss == 'w'){
            printf("プレイヤーの勝ち\n");

        }else if(win_or_loss == 'l'){
            printf("プレイヤーの負け\n");
        }else if(win_or_loss == 'd'){
            printf("引き分け\n");
        }
        printf("\n");
        
        //倍率の計算
        switch (win_or_loss) {
            case 'w':{
                //通常勝利
                return_amount_magnifi = 1.5;
                int n1 = getCardNum(player_owncard[1]);
                int n2 = getCardNum(player_owncard[2]);
                int n3 = getCardNum(player_owncard[3]);
                char s1 = getCardMark(player_owncard[1]);
                char s2 = getCardMark(player_owncard[2]);
                char s3 = getCardMark(player_owncard[3]);
                if(player_scoretotal == 21){
                    if(player_owncard[3] != 0){
                        if(n1 == n2 && n2 == n3 && n1 == 7){
                            //スリーセブン
                            return_amount_magnifi = 7;
                            break;
                        }
                    }
                    if(player_owncard[3] == 0){
                        if(((n1 == 1 && n2 == 11) || (n1 == 11 && n2 == 1)) && s1 == 'S' && s2 == 'S'){
                            //表ブラックジャック
                            return_amount_magnifi = 8;
                            player_cardhand = 1;
                        }else if(((n1 == 1 && n2 == 11) || (n1 == 11 && n2 == 1))){
                            if(!(s1 == 'S' && s2 == 'S')){
                                //裏ブラックジャック
                                return_amount_magnifi = 4;
                                player_cardhand = 2;
                            }
                        }else if((n1 == 1 && n2 >=10) || (n1 >=10 && n2 == 1)){
                            //ブラックジャック
                            return_amount_magnifi = 3;
                            player_cardhand = 3;
                        }
                        break;
                    }
                }
                if(player_owncard[6] != 0){
                    if(player_owncard[7] != 0){
                        //セブンカード
                        return_amount_magnifi = 5;
                        player_cardhand = 4;
                        break;
                    }else{
                        //シックスカード
                        return_amount_magnifi = 3;
                        player_cardhand = 5;
                        break;
                    } 
                }
                break;
            }
            case 'l':{
                return_amount_magnifi = 0;
                break;
            }
            case 'd':{
                return_amount_magnifi = 1;
                break;
            }
        }
        return_amount = return_amount * return_amount_magnifi;

        printf("プレイヤーの手札: ");
        for(int k=0;k<12;k++){
            if(player_owncard[k] != 0){
                cardMark = getCardMark(player_owncard[k]);
                printf("%c", cardMark);
                printf("%d,",getCardNum(player_owncard[k]));
            }
        }
        if(player_cardhand != 0){
            printf("あなたの役：");
            if(player_cardhand == 1){printf("裏ブラックジャック(倍率:%.1f)", return_amount_magnifi);}
            if(player_cardhand == 2){printf("ブラックジャック(倍率:%.1f)", return_amount_magnifi);}
            if(player_cardhand == 3){printf("ブラックジャック(倍率:%.1f)", return_amount_magnifi);}
            if(player_cardhand == 4){printf("セブンカード(倍率:%.1f)", return_amount_magnifi);}
            if(player_cardhand == 5){printf("シックスカード(倍率:%.1f)", return_amount_magnifi);}
            printf("\n");
        }
        printf("(合計: %d)\n",player_scoretotal);
        printf("ディーラーの手札: ");
        for(int k=0;k<12;k++){
            if(dealer_owncard[k] != 0){
                cardMark = getCardMark(dealer_owncard[k]);
                printf("%c", cardMark);
                printf("%d,",getCardNum(dealer_owncard[k]));
            }
        }
        printf("(合計: %d)\n",dealer_scoretotal);

        printf("あなたは$%dをベットしました。\n",betamount);
        printf("倍率: %.1f\n",return_amount_magnifi);
        if(win_or_loss == 'w'){
            printf("配当額: + $%d\n",return_amount);
            printf("所持金:  $%d\n",player_money + betamount + return_amount);
        }else if(win_or_loss == 'l'){
            printf("損失額: -$%d\n",betamount);
            printf("所持金:  $%d\n",player_money);
        }else if(win_or_loss == 'd'){
            printf("配当額: +$0\n");
            printf("所持金:  $%d\n",player_money + return_amount);
        }
        player_money += return_amount;


        //for(int k=0;k<52;k++){printf("%d,",cardstorage_shdc[k]);}

        printf("\nゲームを終了しますか？ (1:はい 0:いいえ) > ");
        scanf("%d",&num_selection);
        if(num_selection == 1){
            exit(0);
        }
        printf("\n");
    }
    return 0;
}

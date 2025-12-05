#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include "BGtext.h"
#define WAR_FUND 300000 //最初の所持金
#define DEBUGMODE 0     //0で通常開始

struct stat{
    int betTtl;
    int returnamountTtl;
    int losingamountTtl;
    int winsTtl;
    int lossTtl;
};

void showPossessionCards(int *possession){
    int length = sizeof(possession) / sizeof(int);
    for(int k=0;k<length;k++){
        printf("%d ",possession[k]);
    }
}

void showEltm(const char *msg){
    printf("%s", msg);
}

char getCardMark(int card){
    int num_mark;
    char mark;
    num_mark = (card - 1) / 13;
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

int getCardNum(int card){
    if(card % 13 == 0){
        return 13;
    }
    return card % 13;
}

int main(void){
    struct stat stat_t = {0,0,0,0,0};
    int player_scoretotal = 0,dealer_scoretotal = 0,hit_count = 0,betamount = 0,return_amount = 0,player_money = WAR_FUND;
    int player_cardhand,num_selection,ace_selection,debugMode = DEBUGMODE;
    float return_amount_magnifi;
    char win_or_loss = 'n',str_selection = 'h',cardMark;
    int player_owncard[12],dealer_owncard[12],cardstorage_shdc[52];
    srand((unsigned)time(NULL));

    while(1){
        int first_employCard[2];
        int HS_employCard[52];
        for(int k = 0;k < 2;k++){
            first_employCard[k] = -1;
        }
        for(int k = 0;k < 52;k++){
            HS_employCard[k] = -1;
        }

        //最初に2枚引くカード(カード番号-1)を指定できる（DEBUGMODEが1のとき）
        first_employCard[0] = 0;first_employCard[1] = 0+13;

        //hit,standで引くカード
        HS_employCard[0] = 0+26;HS_employCard[1] = 0+39;
        HS_employCard[2] = 1;HS_employCard[3] = 1+13;
        HS_employCard[4] = 1+26;HS_employCard[5] = 1+39;

        for(int i = 0;i < 52;i++){
            cardstorage_shdc[i] = i+1;
        }
        player_scoretotal = dealer_scoretotal = 0;
        for(int i = 0;i < 12; i++){
            player_owncard[i] = dealer_owncard[i] = -1;
        }
        str_selection = 'n';
        ace_selection = '0';
        win_or_loss = 'n';
        player_cardhand = 0;
        return_amount = 0;
        hit_count = 0;

        if(player_money < 100){
            printf("\nお金が無くなった!\n");
            usleep(3000000);
            system("clear");
            printf("\n");
            showEltm(messages1[0]);
            printf("\n");
            usleep(3000000);
            showEltm(messages2[0]);
            printf("\n");
            usleep(3000000);
            showEltm(messages3[0]);
            printf("\n");
            usleep(3500000);
            system("clear");
            showEltm(messages4[0]);
            printf("\n");
            usleep(5000000);
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
                stat_t.betTtl += betamount;
                printf("所持金：$%d\n",player_money);
            }
        }while(bet_entry == 0);

        printf("\nプレイヤー、ディーラーに2枚カードを配ります...\n");
        usleep(1000000);
        int rand1 = 0,rand2 = 0,rand3 = 0;
        for(int i = 0;i < 2;i++){
            for(int j = 0;j < 2;j++){
                do{
                    int CanExecuteDebug = 0;
                    for(int k = 0;k < 52;k++){
                        if(first_employCard[k] != -1){
                            CanExecuteDebug = 1;
                        }
                    }
                    if(debugMode == 1 && i == 0 && CanExecuteDebug == 1){                        
                        do{
                            rand3 = rand() % 2;
                            rand1 = first_employCard[rand3];
                        }while(rand1 == -1);
                        first_employCard[rand3] = -1;
                    }else{
                        rand1 = rand() % 52;
                    }
                }while(cardstorage_shdc[rand1] == -1);
                if(i == 0){
                    player_owncard[j] = cardstorage_shdc[rand1];
                    if(getCardNum(cardstorage_shdc[rand1]) == 1 && player_scoretotal+10 <= 21){
                        do{
                            printf("Aceを引きました。(1:'1'としてカウント、2:'11'としてカウント) ");
                            if(j == 1){
                                printf("(1枚目: %d)",getCardNum(player_owncard[0]));
                            }
                            printf(" > ");
                            if(player_scoretotal + 11 > 21){
                                printf("21を超えるため自動的に'1'としてカウントします\n");
                                break;
                            }
                            scanf("%d",&ace_selection);
                        }while(ace_selection != 1 && ace_selection != 2);
                        if(ace_selection == 1 || player_scoretotal + 11 > 21){
                            player_scoretotal += 1;
                        }else{
                            player_scoretotal += 11;
                        }
                    }else if(getCardNum(cardstorage_shdc[rand1]) >= 11 && getCardNum(cardstorage_shdc[rand1]) <= 13){
                        player_scoretotal += 10;
                    }else{
                        player_scoretotal += getCardNum(cardstorage_shdc[rand1]);
                    }
                    cardstorage_shdc[rand1] = -1;
                    if(player_scoretotal == 21 && ((getCardNum(player_owncard[0]) == 1 && getCardNum(player_owncard[1]) >=10) || (getCardNum(player_owncard[0]) >=10 && getCardNum(player_owncard[1]) == 1))){
                        win_or_loss = 'w';
                    }
                }else{
                    dealer_owncard[j] = cardstorage_shdc[rand1];
                    if(getCardNum(cardstorage_shdc[rand1]) == 1 && dealer_scoretotal+10 <= 21){
                        dealer_scoretotal += 11;
                    }else if(getCardNum(cardstorage_shdc[rand1]) >= 11 && getCardNum(cardstorage_shdc[rand1]) <= 13){
                        dealer_scoretotal += 10;
                    }else{
                        dealer_scoretotal += getCardNum(cardstorage_shdc[rand1]);
                    }
                    cardstorage_shdc[rand1] = -1;
                }
            
            }
        }
        if(win_or_loss == 'n'){
            printf("プレイヤーの手札: %c%d, ",getCardMark(player_owncard[0]),getCardNum(player_owncard[0]));
            printf("%c%d (合計: %d)\n ",getCardMark(player_owncard[1]),getCardNum(player_owncard[1]),player_scoretotal);
            printf("ディーラーの手札: %c%d, ? (合計: ?)\n",getCardMark(dealer_owncard[0]),getCardNum(dealer_owncard[0]));
            printf("\n");
            hit_count = 0;
            printf("- プレイヤーのターン -\n");
        }
        while(str_selection != 's'){
            if(win_or_loss == 'w'){break;}
            do{
                int CanExecuteDebug = 0;
                for(int k = 0;k < 52;k++){
                    if(HS_employCard[k] != -1){
                        CanExecuteDebug = 1;
                    }
                }
                if(DEBUGMODE == 1 && CanExecuteDebug == 1){
                    do{
                        rand3 = rand() % 52;
                        rand1 = HS_employCard[rand3];
                    }while(rand1 == -1);                
                }else{
                    rand1 = rand() % 52;
                }
            }while(cardstorage_shdc[rand1] == -1);
            HS_employCard[rand3] = -1;
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
                cardstorage_shdc[rand1] = -1;
                hit_count++;
                printf("手札: ");
                for(int k=0;k<2+hit_count;k++){
                    printf("%c%d,",getCardMark(player_owncard[k]),getCardNum(player_owncard[k]));
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
        if(win_or_loss == 'n'){
            printf("\n- ディーラーのターン -\n");
            usleep(1000000);
        }
        while(dealer_scoretotal < 17){
            if(win_or_loss == 'l' || win_or_loss == 'w'){break;}
            do{
                rand1 = rand() % 52;
            }while(cardstorage_shdc[rand1] == -1);
            dealer_owncard[2 + hit_count] = cardstorage_shdc[rand1];
            if(getCardNum(cardstorage_shdc[rand1]) == 1 && dealer_scoretotal+10 <= 21){
                dealer_scoretotal += 11;
            }else if(getCardNum(cardstorage_shdc[rand1]) >= 11 && getCardNum(cardstorage_shdc[rand1]) <= 13){
                dealer_scoretotal += 10;
            }else{
                dealer_scoretotal += getCardNum(cardstorage_shdc[rand1]);
            }
            cardstorage_shdc[rand1] = -1;
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
            usleep(1000000);

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
        if(win_or_loss == 'w'){
            if(player_scoretotal == 21 && (getCardNum(player_owncard[0]) == 1 && getCardNum(player_owncard[1]) >=10) || (getCardNum(player_owncard[0]) >=10 && getCardNum(player_owncard[1]) == 1)){
                printf("\n最初の手札でプレイヤーがブラックジャックになりました\n");
            }
        }
        printf("-------------------------\n");
        if(win_or_loss == 'w'){
            printf("プレイヤーの勝ち\n");
        }else if(win_or_loss == 'l'){
            printf("プレイヤーの負け\n");
        }else if(win_or_loss == 'd'){
            printf("引き分け\n");
        }
        printf("-------------------------\n");
        
        //倍率の計算
        switch (win_or_loss) {
            case 'w':{
                //通常勝利
                return_amount_magnifi = 1.5;
                int n1 = getCardNum(player_owncard[0]);
                int n2 = getCardNum(player_owncard[1]);
                int n3 = getCardNum(player_owncard[2]);
                char s1 = getCardMark(player_owncard[0]);
                char s2 = getCardMark(player_owncard[1]);
                char s3 = getCardMark(player_owncard[2]);
                if(player_scoretotal == 21){
                    if(player_owncard[2] != -1){
                        if(n1 == n2 && n2 == n3 && n1 == 7){
                            //スリーセブン
                            return_amount_magnifi = 7;
                            player_cardhand = 6;
                            break;
                        }
                    }
                    if(player_owncard[3] == -1){
                        if(((n1 == 1 && n2 == 11) || (n1 == 11 && n2 == 1)) && s1 == 'S' && s2 == 'S'){
                            //表ブラックジャック
                            return_amount_magnifi = 15;
                            player_cardhand = 1;
                            break;
                        }else if(((n1 == 1 && n2 == 11) || (n1 == 11 && n2 == 1))){
                            if(!(s1 == 'S' && s2 == 'S')){
                                //裏ブラックジャック
                                return_amount_magnifi = 4;
                                player_cardhand = 2;
                                break;
                            }
                        }else if((n1 == 1 && n2 >=10) || (n1 >=10 && n2 == 1)){
                            //ブラックジャック
                            return_amount_magnifi = 3;
                            player_cardhand = 3;
                            break;
                        }
                    }
                }
                if(player_owncard[5] != -1){
                    if(player_owncard[6] != -1){
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
            default:{
                return_amount_magnifi = 1;
                break;
            }
        }
        return_amount = return_amount * return_amount_magnifi;
        stat_t.returnamountTtl += return_amount;
        usleep(1000000);

        printf("プレイヤーの手札: ");
        for(int k=0;k<12;k++){
            if(player_owncard[k] != -1){
                cardMark = getCardMark(player_owncard[k]);
                printf("%c", cardMark);
                printf("%d,",getCardNum(player_owncard[k]));
            }
        }
        printf("\n");
        if(win_or_loss == 'w'){
            printf("あなたの役：");
            if(player_cardhand == 0){printf("なし(倍率:%.1f)", return_amount_magnifi);}
            if(player_cardhand == 1){printf("表ブラックジャック(倍率:%.1f)", return_amount_magnifi);}
            if(player_cardhand == 2){printf("裏ブラックジャック(倍率:%.1f)", return_amount_magnifi);}
            if(player_cardhand == 3){printf("ブラックジャック(倍率:%.1f)", return_amount_magnifi);}
            if(player_cardhand == 4){printf("セブンカード(倍率:%.1f)", return_amount_magnifi);}
            if(player_cardhand == 5){printf("シックスカード(倍率:%.1f)", return_amount_magnifi);}
            if(player_cardhand == 6){printf("スリーセブン(倍率:%.1f)", return_amount_magnifi);}
            printf("\n");
        }
        printf(" 合計: %d\n",player_scoretotal);
        printf("\nディーラーの手札: ");
        for(int k=0;k<12;k++){
            if(dealer_owncard[k] != -1){
                cardMark = getCardMark(dealer_owncard[k]);
                printf("%c", cardMark);
                printf("%d,",getCardNum(dealer_owncard[k]));
            }
        }
        printf(" 合計: %d\n",dealer_scoretotal);
        printf("-------------------------\n");
        printf("\t-RESULT-\nあなたは$%dをベットしました。\n",betamount);
        printf("倍率: %.1f\n",return_amount_magnifi);
        if(win_or_loss == 'w'){
            printf("配当額: + $%d\n",return_amount);
            printf("所持金:  $%d\n",player_money + return_amount);
            stat_t.returnamountTtl += return_amount;
            stat_t.winsTtl += 1;
        }else if(win_or_loss == 'l'){
            printf("損失額: -$%d\n",betamount);
            printf("所持金:  $%d\n",player_money);
            stat_t.losingamountTtl += betamount;
            stat_t.lossTtl += 1;
        }else if(win_or_loss == 'd'){
            printf("配当額: +$0\n");
            printf("所持金:  $%d\n",player_money + return_amount);
        }
        printf("-------------------------\n");
        player_money += return_amount;
        usleep(2000000);

        //showPossessionCards(player_owncard);
        //for(int k=0;k<52;k++){printf("%d,",cardstorage_shdc[k]);}

        printf("\nゲームを終了しますか？ (1:はい 0:いいえ) > ");
        scanf("%d",&num_selection);
        if(num_selection == 1){
            printf("\n");
            printf("------------統計------------\n");
            printf("所持金：$%d\n",player_money);
            printf("賭けた総額：$%d\n",stat_t.betTtl);
            printf("勝った総額：$%d\n",stat_t.returnamountTtl);
            printf("負けた総額：$%d\n",stat_t.losingamountTtl);
            printf("勝った総数：%d回\n",stat_t.winsTtl);
            printf("負けた総数：%d回\n",stat_t.lossTtl);
            printf("----------------------------\n");
            exit(0);
        }
        system("clear");
        printf("\n");
    }
    return 0;
}

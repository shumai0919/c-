#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

void drawBoard(int board[9]){
    for (int i = 0; i<9; i++){
        if (board[i] == -1){
            printf(" %d ", i + 1);
        } else if (board[i] != -1) {
            if (board[i] == 0) { printf(" O "); }
            else if (board[i] == 1) { printf(" X "); }
        } else {
            printf("   ");
        }
        if ((i + 1) % 3 != 0) {
            printf("|");
        } else {
            printf("\n");
            if (i != 8) { printf("---+---+---"); }
            printf("\n");
        }
    }
}

int main(void) {
    int sock_id, send_size, recv_size;
    struct sockaddr_in addr;
    struct hostent *hp;
    char buffer[256];
    sock_id = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_id == -1) {
        return -1;
    }
    memset(&addr, 0x00, sizeof(addr));
    hp = gethostbyname("127.0.0.1");
    if (hp == NULL) {
        printf("gethostbyname is NULL\n");
        return -1;
    }
    memcpy(hp->h_addr, (struct sockaddr *)&addr.sin_addr, hp->h_length);
    addr.sin_port = htons(57003); // ポート番号は 27000 + 出席番号
    addr.sin_family = AF_INET;
    if (connect(sock_id, (struct sockaddr *)&addr, sizeof(addr)) != 0) {
        perror("socket");
        printf("connect error: %d\n", errno);
        return -1;
    }

    send_size = send(sock_id, "Hello from client", strlen("Hello from client") + 1, 0);
    if (send_size == 0 || send_size == -1) {
        close(sock_id);
        printf("send error\n");
        return -1;
    }
    printf("connect to server\n");
    recv_size = recv(sock_id, buffer, sizeof(buffer), 0);
    if (recv_size == 0 || recv_size == -1) {
        close(sock_id);
        printf("recv error\n");
        return 0;
    }
    printf("received: %s\n", buffer);
    int player_num;
    recv_size = recv(sock_id, &player_num, sizeof(player_num), 0);//recv プレイヤー番号
    if (recv_size == 0 || recv_size == -1) {
        close(sock_id);
        printf("recv error\n");
        return 0;
    }
    //int player_num = atoi(buffer);
    char player_char = 'O';
    if (player_num == 0) {
        player_char = 'O';
    } else {
        player_char = 'X';
    }

    int select_board, player_turn = 0, total_turn = 0;
    int board[9];
    for (int i = 0; i < 9; i++){
        board[i] = -1;
    }

    while (1) {
        drawBoard(board);
        recv_size = recv(sock_id, &player_turn, sizeof(player_turn), 0);//recv player_turn
        if (recv_size == 0 || recv_size == -1) {
            close(sock_id);
            printf("recv error\n");
            return 0;
        }
        //player_turn = atoi(buffer);
        if (player_turn == player_num) {
            drawBoard(board);
            printf("\n");
            printf("あなたは%cです\n", player_char);
            printf("あなたのターン\n番号で選択 > ");
            scanf("%d", &select_board);

            //入力エラーチェック

            select_board--;
            send_size = send(sock_id, &select_board, sizeof(select_board), 0);//send select board
            if (send_size == 0 || send_size == -1) {
                close(sock_id);
                printf("send error\n");
                return 0;
            }
            recv_size = recv(sock_id, board, sizeof(board), 0);//recv player_turn
            if (recv_size == 0 || recv_size == -1) {
                close(sock_id);
                printf("recv error\n");
                return 0;
            }
        }
    }
    close(sock_id);


    return 0;
}

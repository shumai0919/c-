#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

int winner(int board[9]) {
    int board_order[8][3] = {
        {0,1,2}, {3,4,5}, {6,7,8}, // 横
        {0,3,6}, {1,4,7}, {2,5,8}, // 縦
        {0,4,8}, {2,4,6} // 斜め
    };
    for (int i = 0; i < 8; i++) {
        int a = board_order[i][0];
        int b = board_order[i][1];
        int c = board_order[i][2];

        if (board[a] != -1 && board[a] == board[b] && board[b] == board[c]) {
            return board[a];
        }
    }
    for (int i = 0; i < 9; i++) {
        if (board[i] == -1){
            return -1;
        }
    }
    return 2;
}

int main(void) {
    struct sockaddr_in saddr;
    struct hostent *hp;
    char hostname[256];
    char buffer[256];
    int sock_id, send_size, recv_sock_1,recv_sock_2, recv_size;
    int player_num_buffer;
    srand((unsigned int)time(NULL));
    sock_id = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_id == -1) {
        printf("connect error\n");
        return -1;
    }
    memset(&saddr, 0x00, sizeof(saddr));
    gethostname(hostname, 256);
    hp = gethostbyname(hostname);
    saddr.sin_port = htons(57003); // ポート番号は 27000 + 出席番号
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    int opt = 1;
    setsockopt(sock_id, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if (bind(sock_id, (struct sockaddr *)&saddr, sizeof(saddr)) != 0) {
        close(sock_id);
        printf("bind error\n");
        perror("bind");
        return -1;
    }
    if (listen(sock_id, 1) != 0) {
        close(sock_id);
        printf("listen error\n");
        return -1;
    }
    while (1) {
        recv_sock_1 = accept(sock_id, NULL, NULL);
        if (recv_sock_1 == -1) {
            printf("accept error client:1\n");
            close(sock_id);
            break;
        }
        recv_size = recv(recv_sock_1, buffer, sizeof(buffer), 0);
        if (recv_size == 0 || recv_size == -1) {
            close(recv_sock_1);
            printf("recv error\n");
            break;
        }
        printf("received: %s\n", buffer);
        send_size = send(recv_sock_1, "you are player1", strlen("you are player1") + 1, 0);//send message sock1
        if (send_size == 0 || send_size == -1) {
            close(recv_sock_1);
            printf("send error");
            break;
        }
        player_num_buffer = 0;
        send_size = send(recv_sock_1, &player_num_buffer, sizeof(player_num_buffer), 0);// send player num sock1
        if (send_size == 0 || send_size == -1) {
            close(recv_sock_1);
            printf("send error");
            break;
        }

        recv_sock_2 = accept(sock_id,NULL, NULL);
        if (recv_sock_2 == -1) {
            printf("accept error client:2\n");
            close(sock_id);
            break;
        }
        recv_size = recv(recv_sock_2, buffer, sizeof(buffer), 0);
        if (recv_size == 0 || recv_size == -1) {
            close(recv_sock_2);
            printf("recv error\n");
            break;
        }
        printf("received: %s\n", buffer);

        send_size = send(recv_sock_2, "you are player2", strlen("you are player2") + 1, 0);// send message sock2
        if (send_size == 0 || send_size == -1) {
            close(recv_sock_2);
            printf("send error");
            break;
        }
        player_num_buffer = 1;
        send_size = send(recv_sock_2, &player_num_buffer, sizeof(player_num_buffer), 0);// send player num sock2
        if (send_size == 0 || send_size == -1) {
            close(recv_sock_2);
            printf("send error");
            break;
        }
        
        int win = -1, player_turn = 0, total_turns = 0, select_board;
        int board[9];
        for (int i = 0; i < 9; i++) {
            board[i] = -1;
        }
        char sock_turn[256];

        while (1) {
            send_size = send(recv_sock_1, &player_turn, sizeof(player_turn), 0);// send turn sock1
            if (send_size == 0 || send_size == -1) {
                close(recv_sock_1);
                printf("send error");
                return -1;
            }
            send_size = send(recv_sock_2, &player_turn, sizeof(player_turn), 0);// send turn sock2
            if (send_size == 0 || send_size == -1) {
                close(recv_sock_2);
                printf("send error");
                return -1;
            }

            if (player_turn == 0) {
                recv_size = recv(recv_sock_1, &select_board, sizeof(select_board), 0);// recv client board select sock1
                if (send_size == 0 || send_size == -1) {
                    close(recv_sock_1);
                    printf("recv error");
                    return -1;
                }
            } else {
                recv_size = recv(recv_sock_2, &select_board, sizeof(select_board), 0);// recv client board select sock2
                if (send_size == 0 || send_size == -1) {
                    close(recv_sock_2);
                    printf("recv error");
                    return -1;
                }
                
            }

            //入力エラーチェック
            
            if (player_turn == 0) {
                board[select_board] = 0;
                player_turn = 1;
            } else {
                board[select_board] = 1;
                player_turn = 0;
            }
            send_size = send(recv_sock_1, board, sizeof(board), 0);// send new board sock2
            if (send_size == 0 || send_size == -1) {
                close(recv_sock_1);
                printf("send error");
                return -1;
            }
            send_size = send(recv_sock_2, board, sizeof(board), 0);
            if (send_size == 0 || send_size == -1) {
                close(recv_sock_2);
                printf("send error");
                return -1;
            }
            if (player_turn == 0) {
                player_turn = 0;
            } else {
                player_turn = 1;
            }

            win = winner(board);   
            

            send_size = send(recv_sock_1, &win, sizeof(win), 0);
            if (send_size == 0 || send_size == -1) {
                close(recv_sock_1);
                printf("send error");
                return -1;
            }
            send_size = send(recv_sock_2, &win, sizeof(win), 0);
            if (send_size == 0 || send_size == -1) {
                close(recv_sock_2);
                printf("send error");
                return -1;
            }
            
            if (win == 0 || win == 1){
                printf("win: %d", win);
                break;
            }
            if (win == 2){
                printf("draw");
                break;
            }
        }
        close(recv_sock_1);
    }
    return 0;
}
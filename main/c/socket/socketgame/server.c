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

int RNG(int min, int max){
    return rand() % (max - min + 1) + min;
}

int main(void) {
    struct sockaddr_in saddr;
    struct hostent *hp;
    char hostname[256];
    char buffer[256];
    int sock_id, send_size, recv_sock_1,recv_sock_2, recv_size;
    srand((unsigned int)time(NULL));
    sock_id = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_id == -1) {
        return -1;
    }
    memset(&saddr, 0x00, sizeof(saddr));
    gethostname(hostname, 256);
    hp = gethostbyname(hostname);
    saddr.sin_port = htons(57003); // ポート番号は 27000 + 出席番号
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    //int opt = 1;
    //setsockopt(sock_id, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if (bind(sock_id, (struct sockaddr *)&saddr, sizeof(saddr)) != 0) {
        close(sock_id);
        return -1;
    }
    if (listen(sock_id, 1) != 0) {
        close(sock_id);
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

        send_size = send(recv_sock_1, "you are player1", strlen("you are player1") + 1, 0);
        if (send_size == 0 || send_size == -1) {
            close(recv_sock_1);
            printf("send error");
            break;
        }
        send_size = send(recv_sock_1, "1", strlen("!") + 1, 0);
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
        send_size = send(recv_sock_2, "you are player2", strlen("you are player2") + 1, 0);
        if (send_size == 0 || send_size == -1) {
            close(recv_sock_1);
            printf("send error");
            break;
        }
        send_size = send(recv_sock_2, "2", strlen("2") + 1, 0);
        if (send_size == 0 || send_size == -1) {
            close(recv_sock_1);
            printf("send error");
            break;
        }

        int win = 0, player_turn = 1, total_turns = 0;
        int board[9] = {0};

        while (1) {
            send(recv_sock_1, &player_turn, sizeof(player_turn), 0);
            if (send_size == 0 || send_size == -1) {
                close(recv_sock_1);
                printf("send error");
                break;
            }
            send(recv_sock_2, &player_turn, sizeof(player_turn), 0);
            if (send_size == 0 || send_size == -1) {
                close(recv_sock_1);
                printf("send error");
                break;
            }

            if (turn == 1){

            }

        }

        close(recv_sock_1);
    }
    return 0;
}

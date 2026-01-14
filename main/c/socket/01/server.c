#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

int RNG(int max, int min){
    return rand() % (max - min + 1) + min;
}

int main(void) {
    struct sockaddr_in saddr;
    struct hostent *hp;
    char hostname[256];
    char buffer[256];
    int sock_id, recv_sock, recv_size;
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
        recv_sock = accept(sock_id, NULL, NULL);
        if (recv_sock == -1) {
            printf("accept error\n");
            close(sock_id);
            break;
        }
        recv_size = recv(recv_sock, buffer, sizeof(buffer), 0);
        if (recv_size == 0 || recv_size == -1) {
            close(recv_sock);
            printf("recv error\n");
            break;
        }
        printf("received: %s\n", buffer);

        send(recv_sock, "I'm Server", strlen("I'm Server") + 1, 0);

        recv_size = recv(recv_sock, buffer, sizeof(buffer), 0);
        if (recv_size == 0 || recv_size == -1) {
            close(recv_sock);
            printf("recv error\n");
            break;
        }
        printf("received: %s\n", buffer);
        send(recv_sock, RNG(0, 100),

        close(recv_sock);
    }
    return 0;
}
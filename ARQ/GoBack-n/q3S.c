#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8080
#define WINDOW_SIZE 4
#define TOTAL_FRAMES 10
#define TIMEOUT 3

int main() {
    int sockfd;
    struct sockaddr_in serv_addr;
    char frame[2], ack[2];
    int base = 0, next_seq = 0;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to receiver.\n");

    while (base < TOTAL_FRAMES) {
        while (next_seq < base + WINDOW_SIZE && next_seq < TOTAL_FRAMES) {
            sprintf(frame, "%d", next_seq);
            printf("Sending Frame: %s\n", frame);
            send(sockfd, frame, strlen(frame), 0);
            next_seq++;
        }

        struct timeval tv;
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(sockfd, &readfds);
        tv.tv_sec = TIMEOUT;
        tv.tv_usec = 0;

        int activity = select(sockfd + 1, &readfds, NULL, NULL, &tv);

        if (activity > 0) {
            recv(sockfd, ack, sizeof(ack), 0);
            printf("Received ACK: %s\n", ack);
            base = atoi(ack) + 1;
        } else {
            printf("Timeout! Resending from Frame: %d\n", base);
            next_seq = base;
        }
    }

    printf("All frames sent successfully.\n");
    close(sockfd);
    return 0;
}

